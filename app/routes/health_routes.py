from fastapi import APIRouter
from app.logger import logger
from dto.dto_obj import SuccessDTO, ErrorDTO

router = APIRouter(prefix="/health")


@router.get("/health_check")
def health_check():
    try:
        logger.debug("health_check -> start")
        return SuccessDTO(message="Health check successful", data="Healthy")
    
    except Exception as e:
        logger.exception(f"health_check -> error: {str(e)}")
        return ErrorDTO(message="Failed to check health")