"""
Feed source text to ./main via stdin, parse
and compare.

To run:

    pip install pytest
    pytest -v
"""

import subprocess
import pathlib
import pytest

ROOT = pathlib.Path(__file__).resolve().parent
BINARY = ROOT / "main"

@pytest.fixture(scope="session", autouse=True)
def build():
    res = subprocess.run(["make"], cwd=ROOT, capture_output=True, text=True)
    if res.returncode != 0:
        pytest.fail(f"make failed:\n{res.stdout}\n{res.stderr}")
    if not BINARY.exists():
        pytest.fail("make succeeded but ./main was not produced.")


def lex(src):
    res = subprocess.run(
        [str(BINARY)], input=src, capture_output=True, text=True, cwd=ROOT
    )
    tokens = []
    for raw in res.stdout.splitlines():
        parts = raw.split()
        if not parts:
            continue
        line_no = int(parts[0])
        kind = parts[1]
        payload = parts[2] if len(parts) > 2 else None
        tokens.append((kind, payload, line_no))
    return tokens

def stream(src):
    out = [(k, p) for (k, p, _in) in lex(src)]
    if out and out[-1][0] == "EOF":
        out = out[:-1]
    return out

CASES = [
    (
        "basic_keywords_and_semicolon",
        "12 div 5;",
        [("NUMBER", "12"), ("DIV", "div"), ("NUMBER", "5"), ("SEMICOLON", None)],
    ),
    (
        "whitespace_is_significant",
        "12div5",
        [("NUMBER", "12"), ("IDENTIFIER", "div5")],
    ),
    (
        "keywords_are_not_prefixes",
        "division divmod moddiv",
        [("IDENTIFIER", "division"), ("IDENTIFIER", "divmod"), ("IDENTIFIER", "moddiv")],
    ),
    (
        "underscore_asymmetry",
        "_total total_ _",
        [("UNKNOWN", None), ("IDENTIFIER", "total"), ("IDENTIFIER", "total_"), ("UNKNOWN", None)],
    ),
    (
        "minus_is_always_its_own_token",
        "-5 + -x",
        [("MINUS", None), ("NUMBER", "5"), ("PLUS", None), ("MINUS", None), ("IDENTIFIER", "x")],
    ),
    (
        "minus_is_always_its_own_token",
        "-5 + -x",
        [("MINUS", None), ("NUMBER", "5"), ("PLUS", None),
         ("MINUS", None), ("IDENTIFIER", "x")],
    ),
    (
        "slash_and_div_coexist",
        "a / b div c",
        [("IDENTIFIER", "a"), ("SLASH", None), ("IDENTIFIER", "b"),
         ("DIV", "div"), ("IDENTIFIER", "c")],
    ),
    (
        "nested_parens",
        "((a+b)*c) div d;",
        [("LPAREN", None), ("LPAREN", None), ("IDENTIFIER", "a"), ("PLUS", None),
         ("IDENTIFIER", "b"), ("RPAREN", None), ("STAR", None), ("IDENTIFIER", "c"),
         ("RPAREN", None), ("DIV", "div"), ("IDENTIFIER", "d"), ("SEMICOLON", None)],
    ),
    (
        "leading_zeros_collapse",
        "007 + 42",
        [("NUMBER", "7"), ("PLUS", None), ("NUMBER", "42")],
    ),
    (
        "multichar_operators_split_into_unknowns",
        "a == b @ c",
        [("IDENTIFIER", "a"), ("UNKNOWN", None), ("UNKNOWN", None),
         ("IDENTIFIER", "b"), ("UNKNOWN", None), ("IDENTIFIER", "c")],
    ),
]

@pytest.mark.parametrize(
    "src,expected", [(c[1], c[2]) for c in CASES], ids=[c[0] for c in CASES]
)
def test_token_stream(src, expected):
    assert stream(src) == expected

def test_empty_input_is_just_eof():
    assert lex("")[0][0] == "EOF"



def test_line_numbers_track_across_newlines():
    src = "a +\n  b *\n    c;\n"
    assert lex(src)[:6] == [
        ("IDENTIFIER", "a", 1),
        ("PLUS", None, 1),
        ("IDENTIFIER", "b", 2),
        ("STAR", None, 2),
        ("IDENTIFIER", "c", 3),
        ("SEMICOLON", None, 3),
    ]



