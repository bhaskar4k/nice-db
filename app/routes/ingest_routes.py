from fastapi import APIRouter
from app.services.ingest_service import create_ingestion_job

router = APIRouter(prefix="/ingest")


@router.post("/ingest_data")
def ingest_data(request):
    job_id = create_ingestion_job(request)
    return {"job_id": job_id}