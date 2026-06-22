GroqRAG_Project
A robust, document-based Question & Answering system that bridges a C++ Qt interface with a powerful Python-based RAG (Retrieval-Augmented Generation) backend. This project was developed as part of an Object-Oriented Programming (OOP) semester project at NED University of Engineering and Technology.

🚀 Overview
This application allows users to upload PDF documents and query them using an AI-powered assistant. The system uses FAISS for efficient vector storage and search, and integrates the Groq API (powered by Llama 3.1) to generate context-aware, accurate answers.

🛠 Tech Stack
Frontend: C++ (Qt Framework)

Backend: Python

AI/ML: LangChain, HuggingFace Embeddings (all-MiniLM-L6-v2), FAISS

LLM: Llama 3.1 (via Groq API)

Version Control: Git

📂 Key Features
PDF Ingestion: Extracts and chunks text from PDF files.

Vector Search: Utilizes local FAISS indexing to find relevant document sections.

Intelligent Q&A: Leverages Llama 3.1 for natural language understanding.

Modular Architecture: Clean separation between the C++ UI and the Python backend engine.

⚙️ Setup & Installation
Prerequisites
Python 3.x

Qt Creator (for the C++ frontend)

An API Key from Groq Cloud

Environment Configuration
The application requires your Groq API key to function. Set it in your environment:

Windows (CMD):

DOS
set GROQ_API_KEY=your_api_key_here
Running the Backend
The backend can be interacted with via command line arguments:

Bash
# To process a PDF
python backend/rag_engine.py --upload "path/to/your/document.pdf"

# To ask a question
python backend/rag_engine.py --ask "What is the summary of this document?"
📝 Project Structure
backend/: Contains the core logic (rag_engine.py) for processing PDFs and communicating with the LLM.

mainwindow.cpp / .h: Handles the C++ Qt UI interactions.

faiss_index/: (Git-ignored) Stores the vector database locally.

🎓 Academic Context
This project was developed during my second semester at NED University of Engineering & Technology as a practical application of object-oriented design and modern AI systems.
# GroqRAG Project: From Static Pipelines to Agentic Consensus

This repository contains the source code for a fully functional baseline RAG system built using LangChain, FAISS, and Llama 3.1 via Groq. 

## 📄 New Architectural Blueprint (June 2026)
I have recently published a technical whitepaper detailing the structural limitations of this baseline and proposing a self-correcting, multi-agent RAG framework.

* **Read the Whitepaper:** [Beyond Static Context: Architecting a General-Purpose, Self-Correcting RAG Platform](./docs/whitepaper_final_v2.pdf)
* **Status:** Baseline system is fully functional (see code below). Advanced agentic architecture implementation is currently underway.

---

Created by MARIUM TARIQ and NIDA NADEEM 
