from metadata.job_repository import create_job


def create_ingestion_job(request):

    job_id = create_job(
        table_name=request.table,
        file_path=request.path
    )

    return job_id