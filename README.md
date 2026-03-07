# NICE-DB: Data Ingestion Engine

A high-performance, asynchronous data ingestion platform built with FastAPI, designed to handle large-scale data processing and storage into Delta Lake format.

## Table of Contents

- [Overview](#overview)
- [Features](#features)
- [Architecture](#architecture)
- [Prerequisites](#prerequisites)
- [Installation](#installation)
- [Configuration](#configuration)
- [Running the Application](#running-the-application)
- [API Documentation](#api-documentation)
- [Project Structure](#project-structure)
- [Development](#development)

## Overview

NICE-DB is a robust data ingestion engine that provides a REST API for ingesting data files, processing them asynchronously, and storing results in Delta Lake format. It's designed for scalability and reliability with support for multiple data formats and comprehensive job management.

## Features

- **FastAPI REST API**: Modern, type-safe API endpoints for data ingestion
- **Asynchronous Processing**: Non-blocking request handling with background worker processing
- **Delta Lake Integration**: Efficient columnar data storage with ACID transactions
- **Job Management**: Track ingestion jobs with status monitoring and history
- **Health Monitoring**: Built-in health checks for service availability verification
- **Polars Integration**: High-performance data manipulation using Polars DataFrames
- **Scalable Architecture**: Worker threads for handling parallel ingestion tasks

## Architecture

The application follows a layered architecture:

```
┌─────────────────────────────────────────────┐
│           FastAPI Application               │
│    (Health & Ingestion Routes)              │
├─────────────────────────────────────────────┤
│        Ingestion Service Layer              │
│    (Job creation & coordination)            │
├─────────────────────────────────────────────┤
│         Metadata & Storage Layers           │
│  (Job Repository, Database, Delta Writer)   │
├─────────────────────────────────────────────┤
│   Worker Layer (Background Processing)      │
│      (External worker instances)            │
└─────────────────────────────────────────────┘
```

## Prerequisites

- Python 3.8 or higher
- pip package manager
- SQLite3 (or compatible database)

## Installation

1. **Clone the repository**
   ```bash
   git clone <repository-url>
   cd NICE-DB
   ```

2. **Create a virtual environment** (recommended)
   ```bash
   python -m venv venv
   venv\Scripts\activate  # On Windows
   source venv/bin/activate  # On macOS/Linux
   ```

3. **Install dependencies**
   ```bash
   pip install -r requirements.txt
   ```

## Configuration

### Environment Variables

Create a `.env` file in the project root with the following settings:

```env
# API Settings
API_HOST=127.0.0.1
API_PORT=8000
API_RELOAD=true

# Database Settings
DATABASE_URL=sqlite:///./nice_db.db

# Worker Settings
WORKER_THREADS=4
WORKER_BATCH_SIZE=100
```

### Database Setup

The application uses SQLite by default. Tables are automatically created on first run. Key tables include:

- `ingestion_jobs`: Tracks all ingestion jobs
- `job_logs`: Stores job execution logs

## Running the Application

### Development Mode

```bash
python main.py
```

The API will be available at `http://localhost:8000`

### Production Mode

```bash
uvicorn app.app:app --host 0.0.0.0 --port 8000 --workers 4
```

### Running Background Workers

In a separate terminal:

```bash
python -m worker.worker
```

## API Documentation

### API Base URL

```
http://localhost:8000
```

### Endpoints

#### 1. Health Check

**Endpoint:** `GET /health/health_check`

**Description:** Verify application health and availability

**Response:**
```json
{
  "status": "healthy"
}
```

**Example:**
```bash
curl http://localhost:8000/health/health_check
```

---

#### 2. Ingest Data

**Endpoint:** `POST /ingest/ingest_data`

**Description:** Submit a new data ingestion job

**Request Body:**
```json
{
  "table": "target_table_name",
  "path": "/path/to/data/file.parquet"
}
```

**Response:**
```json
{
  "job_id": "unique_job_identifier"
}
```

**Example:**
```bash
curl -X POST http://localhost:8000/ingest/ingest_data \
  -H "Content-Type: application/json" \
  -d '{
    "table": "sales_data",
    "path": "/data/sales.parquet"
  }'
```

**Status Codes:**
- `200 OK`: Job created successfully
- `400 Bad Request`: Invalid request parameters
- `500 Internal Server Error`: Server-side error

---

### Response Codes

| Code | Description |
|------|-------------|
| 200  | Success |
| 201  | Resource created |
| 400  | Bad request - invalid parameters |
| 404  | Resource not found |
| 500  | Internal server error |

### Interactive API Documentation

FastAPI automatically generates interactive API documentation:

- **Swagger UI**: http://localhost:8000/docs
- **ReDoc**: http://localhost:8000/redoc

## Project Structure

```
NICE-DB/
├── main.py                      # Application entry point
├── requirements.txt             # Python dependencies
├── README.md                    # This file
├── .gitignore                   # Git ignore rules
│
├── app/                         # FastAPI application
│   ├── app.py                   # FastAPI app factory
│   ├── config.py                # Configuration settings
│   ├── models/                  # Data models
│   │   └── job_model.py         # Job schema definitions
│   ├── routes/                  # API route handlers
│   │   ├── health_routes.py     # Health check endpoints
│   │   └── ingest_routes.py     # Data ingestion endpoints
│   └── services/                # Business logic
│       └── ingest_service.py    # Ingestion job service
│
├── metadata/                    # Data persistence layer
│   ├── db.py                    # Database connection management
│   └── job_repository.py        # Job data access layer
│
├── storage/                     # Storage layer
│   └── delta_writer.py          # Delta Lake writer implementation
│
├── ingestion/                   # Ingestion logic
│   └── worker_logic.py          # Core ingestion processing
│
└── worker/                      # Background worker
    └── worker.py                # Worker process entry point
```

### File Descriptions

| File | Purpose |
|------|---------|
| `main.py` | Entry point that starts the uvicorn server |
| `app/app.py` | FastAPI application factory and router configuration |
| `app/routes/` | API endpoint definitions |
| `app/services/ingest_service.py` | Coordinates ingestion job creation and queuing |
| `metadata/job_repository.py` | Database operations for job management |
| `storage/delta_writer.py` | Writes processed data to Delta Lake format |
| `worker/worker.py` | Background worker for async job processing |

## Development

### Setting Up for Development

1. Install development dependencies:
   ```bash
   pip install -r requirements.txt
   # Add development tools as needed
   ```

2. Run tests (when implemented):
   ```bash
   pytest tests/
   ```

### Code Style

- Follow PEP 8 guidelines
- Use type hints for function parameters and return values
- Document public functions with docstrings

### Making Changes

1. Create a feature branch
2. Make your changes
3. Test thoroughly
4. Submit a pull request

## Dependencies

| Package | Version | Purpose |
|---------|---------|---------|
| fastapi | 0.111.0 | Web framework |
| uvicorn | 0.30.1 | ASGI server |
| polars | 0.20.31 | Data manipulation |
| deltalake | 0.18.2 | Delta Lake support |
| pydantic | 2.7.1 | Data validation |
| python-multipart | 0.0.9 | Form data parsing |

## Troubleshooting

### Application won't start
- Verify Python version is 3.8+
- Check all dependencies are installed: `pip install -r requirements.txt`
- Ensure port 8000 is available

### Worker process not processing jobs
- Check worker process is running in a separate terminal
- Verify database connection is working
- Check application logs for error messages

### Database errors
- Ensure database file has write permissions
- Check disk space availability
- Verify database connection string in `.env`

## License

(Add your license information here)

## Support

For issues, questions, or contributions, please contact the development team or open an issue in the repository.