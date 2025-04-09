#!/usr/bin/env python3
# Copyright (C) 2025 The Qt Company Ltd.
# SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only WITH Qt-GPL-exception-1.0

# /// script
# dependencies = ["check-jsonschema", "click"]
# ///

from __future__ import annotations

from pathlib import Path

import click


@click.command(
    context_settings=dict(
        ignore_unknown_options=True,
    )
)
@click.option(
    "--install-prefix",
    type=click.Path(
        exists=True,
        file_okay=False,
        dir_okay=True,
        path_type=Path,
    ),
    required=True,
    metavar="PATH",
    help="Path to the Qt install prefix.",
)
@click.argument(
    "check_jsonschema_args",
    nargs=-1,
    type=click.UNPROCESSED,
)
def run(install_prefix: Path, check_jsonschema_args: list[str]):
    """
    Validate the module json files after installation.

    Unknown options are passed directly to check-jsonschema.
    """
    # TODO: Do the actual validation


if __name__ == "__main__":
    run()
