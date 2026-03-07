from fastapi import APIRouter
from app.logger import logger

router = APIRouter(prefix="/health")


@router.get("/health_check")
def health_check():
    try:
        logger.debug("health_check -> start")
        return {
            "status": "healthy"
        }
    
    except Exception as e:
        logger.exception(f"health_check -> error: {str(e)}")
        raise