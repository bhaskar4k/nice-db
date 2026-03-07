from fastapi import APIRouter
from app.services.ingest_service import create_ingestion_job
from app.models.job_model import IngestionRequest

router = APIRouter(prefix="/ingest")


@router.post("/ingest_data")
def ingest_data(request: IngestionRequest):
    print(f"Received ingest request: {request}")
    job_id = create_ingestion_job(request)
    return {"job_id": job_id}