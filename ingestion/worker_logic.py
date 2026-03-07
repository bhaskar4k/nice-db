from storage.delta_writer import write_delta
from polars import pl


def fetch_pending_job():
    return None


def process_job(job):

    df = pl.scan_csv(job.file_path)

    for batch in df.collect(streaming=True):
        write_delta(batch)