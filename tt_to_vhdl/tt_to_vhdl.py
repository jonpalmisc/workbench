from typing import Dict, List

import pandas as pd


def row_sop(row: pd.Series, in_cols: List[str], out_col: str) -> List[str]:
    """
    Calculate a sum-of-products for a single column using one row in a dataset.

    :param row: Row of data to use
    :param in_cols: List of input column keys (the terms of each product)
    :param out_col: Column key to calculate for
    """
    if row[out_col] != "1":
        return []

    terms: List[str] = []
    for col in in_cols:
        match row[col]:
            case "1":
                terms.append(f"{col}")
            case "0":
                terms.append(f"(not {col})")
            case "*":
                pass
            case _:
                raise RuntimeError(f"unsupported value '{row[col]}' in column '{col}'")

    return [t for t in filter(lambda u: len(u) > 0, terms)]


def col_sop(df: pd.DataFrame, in_cols: List[str], out_col: str) -> List[List[str]]:
    """
    Calculate a sum-of-products for a single column using all rows in a dataset.

    :param df: Data source to use
    :param in_cols: List of input column keys (the terms of each product)
    :param out_col: Column key to calculate for (the product)
    """

    all_sops = [row_sop(row, in_cols, out_col) for (_, row) in df.iterrows()]
    return [sop for sop in filter(lambda s: len(s) > 0, all_sops)]


def table_sop(
    df: pd.DataFrame, in_cols: List[str], out_cols: List[str]
) -> Dict[str, List[str]]:
    """
    Calculate a sum-of-products for an entire dataset.

    :param df: Data source to use
    :param in_cols: List of input column keys (the terms of each product)
    :param out_cols: List of output column keys (the products)
    """

    sops: Dict[str, List[str]] = {}
    for col in out_cols:
        sops[col] = col_sop(df, in_cols, col)

    return sops
