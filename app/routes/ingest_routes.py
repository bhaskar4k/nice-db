from fastapi import APIRouter
from app.services.ingest_service import create_ingestion_job, get_all_ingestion_jobs, get_the_first_pending_ingestion_job
from app.models.job_model import IngestionRequest
from app.logger import logger
from dto.dto_obj import SuccessDTO, ErrorDTO

router = APIRouter(prefix="/ingest")


@router.post("/ingest_data")
def ingest_data(request: IngestionRequest):
    try:
        logger.info(f"ingest_data -> start")

        job_created = create_ingestion_job(request)

        logger.info(f"ingest_data -> end")
        return SuccessDTO(message="Ingestion job created successfully", data=job_created)
    
    except Exception as e:
        logger.exception(f"ingest_data -> error: {str(e)}")
        return ErrorDTO(message="Failed to create ingestion job")
    


@router.get("/get_all_jobs")
def get_all_jobs():
    try:
        logger.info(f"get_all_jobs -> start")
        
        jobs = get_all_ingestion_jobs()
        
        logger.info(f"get_all_jobs -> end")
        return SuccessDTO(message="Ingestion jobs retrieved successfully", data={"count": len(jobs), "jobs": jobs})
    
    except Exception as e:
        logger.exception(f"get_all_jobs -> error: {str(e)}")
        return ErrorDTO(message="Failed to retrieve ingestion jobs")
    


@router.get("/get_first_pending_job")
def get_first_pending_job():
    try:
        logger.info(f"get_first_pending_job -> start")
        
        job = get_the_first_pending_ingestion_job()
        
        logger.info(f"get_first_pending_job -> end")
        return SuccessDTO(message="First pending ingestion job retrieved successfully", data=job)
    
    except Exception as e:
        logger.exception(f"get_first_pending_job -> error: {str(e)}")
        return ErrorDTO(message="Failed to retrieve first pending ingestion job")