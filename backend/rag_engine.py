```python
import sys
import os
import warnings

warnings.filterwarnings("ignore")

from langchain_community.document_loaders import PyPDFLoader
from langchain_text_splitters import RecursiveCharacterTextSplitter
from langchain_community.embeddings import HuggingFaceEmbeddings
from langchain_community.vectorstores import FAISS

from langchain_groq import ChatGroq
from langchain_core.prompts import ChatPromptTemplate
from langchain_core.output_parsers import StrOutputParser
from langchain_core.runnables import RunnablePassthrough


class GroqRAGBackend:
    def __init__(self):

        # Get API key from environment variable
        self.groq_api_key = os.getenv("GROQ_API_KEY")

        if not self.groq_api_key:
            raise ValueError(
                "GROQ_API_KEY environment variable not found. "
                "Please set it before running the application."
            )

        os.environ["GROQ_API_KEY"] = self.groq_api_key

        # Local Embedding Model
        self.embeddings = HuggingFaceEmbeddings(
            model_name="all-MiniLM-L6-v2"
        )

        self.vector_store_path = "faiss_index"

        self.llm = ChatGroq(
            temperature=0.2,
            model_name="llama-3.1-8b-instant"
        )

    def process_document(self, file_path):
        print(f"System: Processing '{file_path}'...")

        if not os.path.exists(file_path):
            print(f"Error: File '{file_path}' not found!")
            return

        loader = PyPDFLoader(file_path)
        documents = loader.load()

        text_splitter = RecursiveCharacterTextSplitter(
            chunk_size=1000,
            chunk_overlap=200
        )

        chunks = text_splitter.split_documents(documents)

        print("System: Generating vector embeddings... (Please wait)")

        self.vector_store = FAISS.from_documents(
            chunks,
            self.embeddings
        )

        self.vector_store.save_local(self.vector_store_path)

        print("System: SUCCESS! Document loaded and vector index saved.")

    def ask_question(self, query):

        if not os.path.exists(self.vector_store_path):
            print("Error: No document uploaded yet. Please upload a PDF first.")
            return

        vector_store = FAISS.load_local(
            self.vector_store_path,
            self.embeddings,
            allow_dangerous_deserialization=True
        )

        retriever = vector_store.as_retriever(
            search_kwargs={"k": 3}
        )

        prompt = ChatPromptTemplate.from_template(
            """
Answer the following question based ONLY on the provided context.

If the answer is not in the context, say:
"I cannot answer this based on the document."

Context:
{context}

Question:
{question}
"""
        )

        def format_docs(docs):
            return "\n\n".join(
                doc.page_content for doc in docs
            )

        rag_chain = (
            {
                "context": retriever | format_docs,
                "question": RunnablePassthrough()
            }
            | prompt
            | self.llm
            | StrOutputParser()
        )

        response = rag_chain.invoke(query)

        print("\n--- AI ANSWER ---")
        print(response)
        print("-----------------\n")


if __name__ == "__main__":

    if len(sys.argv) < 3:
        print("Error: Invalid arguments passed.")
        sys.exit(1)

    command = sys.argv[1]
    payload = sys.argv[2]

    rag = GroqRAGBackend()

    if command == "--upload":
        rag.process_document(payload)

    elif command == "--ask":
        rag.ask_question(payload)
```
