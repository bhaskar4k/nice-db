from pydantic import BaseModel

class IngestionRequest(BaseModel):
    table: str
    path: str