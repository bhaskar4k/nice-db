from typing import Any, Optional
from pydantic import BaseModel


class SuccessDTO(BaseModel):
    status: int = 200
    success: bool = True
    message: str
    data: Optional[Any] = None

    
class ErrorDTO(BaseModel):
    status: int = 500
    success: bool = False
    message: str
    data: Optional[Any] = None


class UnauthorizedDTO(BaseModel):
    status: int = 401
    success: bool = False
    message: str
    data: Optional[Any] = None


class NotFoundDTO(BaseModel):
    status: int = 404
    success: bool = False
    message: str
    data: Optional[Any] = None


class BadRequestDTO(BaseModel):
    status: int = 400
    success: bool = False
    message: str    
    data: Optional[Any] = None


class InternalServerErrorDTO(BaseModel):
    status: int = 500
    success: bool = False
    message: str    
    data: Optional[Any] = None


class ConflictDTO(BaseModel):
    status: int = 409
    success: bool = False
    message: str    
    data: Optional[Any] = None 


class ValidationErrorDTO(BaseModel):
    status: int = 422
    success: bool = False
    message: str    
    data: Optional[Any] = None