def process_job(job):

    df = polars.scan_csv(job.file_path)

    for batch in df.collect(streaming=True):
        write_delta(batch)