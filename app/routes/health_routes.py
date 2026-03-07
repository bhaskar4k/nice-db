from fastapi import APIRouter
from app.services.ingest_service import create_ingestion_job

router = APIRouter(prefix="/health")


@router.get("/health_check")
def health_check():
    return {
        "status": "healthy"
    }