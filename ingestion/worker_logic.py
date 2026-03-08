from ingestion.delta_writer import write_delta
import polars as pl


def process_job(job):
    val = 1
    # df = pl.scan_csv(job.file_path)

    # for batch in df.collect(streaming=True):
    #     write_delta(batch)