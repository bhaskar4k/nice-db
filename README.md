# 🚀 NICE-DB: High-Performance Data Ingestion Engine

> A modern, asynchronous data ingestion platform built with **FastAPI** and **Delta Lake** for large-scale data processing and storage.

[![Python](https://img.shields.io/badge/Python-3.12+-blue.svg)](https://www.python.org/)
[![FastAPI](https://img.shields.io/badge/FastAPI-0.104+-green.svg)](https://fastapi.tiangolo.com/)
<!-- [![License](https://img.shields.io/badge/License-MIT-yellow.svg)](LICENSE) -->

---

## ✨ Overview

**NICE-DB** is a robust, production-ready data ingestion engine that provides a modern REST API for:
- Ingesting files from multiple formats (CSV, Parquet, Excel, TXT)
- Processing data asynchronously without blocking requests
- Storing results efficiently in Delta Lake format
- Managing and tracking jobs with comprehensive status monitoring

Built with scalability and reliability in mind, it supports parallel processing with configurable worker threads and includes comprehensive logging and health monitoring.

---

## 🎯 Features

| Feature | Description |
|---------|-------------|
| 🔌 **FastAPI REST API** | Type-safe, auto-documented REST endpoints for easy integration |
| ⚡ **Asynchronous Processing** | Non-blocking request handling with background job execution |
| 📊 **Delta Lake Integration** | Efficient columnar storage with ACID transactions |
| 📈 **Job Management** | Complete job lifecycle tracking with status monitoring |
| 🏥 **Health Monitoring** | Built-in health checks for service availability verification |
| 🐻 **Polars Integration** | High-performance data manipulation with Polars DataFrames |
| 🔄 **Scalable Architecture** | Configurable worker threads with semaphore-based concurrency control |
| 🔐 **Thread-Safe Operations** | Mutex locks and semaphores to prevent deadlocks and race conditions |
| 📝 **Comprehensive Logging** | File-based logging with rotation and exception tracking |
| ⚙️ **Configurable Settings** | JSON-based configuration for easy deployment customization |

---

## 🏗️ Architecture

NICE-DB follows a **layered microservices architecture** for maximum separation of concerns:
```
┌─────────────────────────────────────────────────────────────┐
│                   FastAPI Web Server                        │
│            (Health Check & Ingestion Routes)                │
├─────────────────────────────────────────────────────────────┤
│                   Ingestion Service Layer                   │
│               (Job Coordination & Validation)               │
├─────────────────────────────────────────────────────────────┤
│          Metadata & Storage Service Layers                  │
│       (Repository, Database ORM, Delta Writer)              │
├─────────────────────────────────────────────────────────────┤
│           Background Worker Layer (APScheduler)             │
│        (Cron Jobs, Threading, Concurrency Control)          │
├─────────────────────────────────────────────────────────────┤
│                  Data Storage Layer                         │
│            (Delta Lake, SQLite Metadata DB)                 │
└─────────────────────────────────────────────────────────────┘
```

---

## 📋 Prerequisites

- **Python**: 3.12 or higher
- **Package Manager**: pip
- **Database**: SQLite3 (included with Python)
- **OS**: Windows, macOS, or Linux

---

## 📦 Installation

### Step 1: Clone the Repository

```bash
git clone https://github.com/bhaskar4k/nice-db.git
cd nice-db
```

### Step 2: Create Virtual Environment (Recommended)

**Windows:**
```bash
python -m venv venv
venv\Scripts\activate
```

**macOS/Linux:**
```bash
python -m venv venv
source venv/bin/activate
```

### Step 3: Install Dependencies

```bash
pip install --upgrade pip
pip install -r requirements.txt
```

---

## 🏃 Running the Application

### Start the Server

```bash
python main.py
```

**Expected Output:**
```
Starting worker scheduler in background
Scheduler started - job runs every 30 seconds
Worker scheduler started in background
INFO:     Started server process [13536]
INFO:     Waiting for application startup.
INFO:     Application startup complete.
INFO:     Uvicorn running on http://127.0.0.1:8000 (Press CTRL+C to quit)
```

### API Base URL

```
http://localhost:8000
```

---

## 📚 API Documentation

### Interactive Documentation

FastAPI provides automatic interactive API documentation:

- **Swagger UI**: [http://localhost:8000/docs](http://localhost:8000/docs)
- **ReDoc**: [http://localhost:8000/redoc](http://localhost:8000/redoc)

### API Endpoints

#### 1️⃣ Health Check

**Endpoint:** `GET /health/health_check`

**Description:** Verify application health and availability

**Response:**
```json
{
  "status": 200,
  "success": true,
  "message": "Health check successful",
  "data": "Healthy"
}
```

#### 2️⃣ Ingest Data

**Endpoint:** `POST /ingest/ingest_data`

**Description:** Submit a new data ingestion job

**Request Body:**
```json
{
  "table": "target_table_name",
  "path": "/path/to/data/file.excel/csv/parquet/txt"
}
```

**Response:**
```json
{
    "status": 200,
    "success": true,
    "message": "Ingestion job created successfully",
    "data": true
}
```

---

## 🔧 Development

### Development Requirements

```bash
pip install pytest pytest-cov black flake8
```

### Code Quality

**Format Code:**
```bash
black .
```

**Lint Code:**
```bash
flake8 .
```

### Running Tests

```bash
pytest tests/ -v --cov=app
```

---

## 💡 Contributing

Contributions are welcome! Please follow these steps:

1. Fork the repository
2. Create a feature branch (`git checkout -b feature/amazing-feature`)
3. Commit your changes (`git commit -m 'Add amazing feature'`)
4. Push to the branch (`git push origin feature/amazing-feature`)
5. Open a Pull Request

---

## 📞 Support
For issues, questions, or contributions, please contact the development team or create an issue in the repository.

---

<div align="center">

**Made with ❤️ by the NICE-DB Team**

[⬆ Back to Top](#-nicedb-high-performance-data-ingestion-engine)

</div>