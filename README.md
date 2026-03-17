# computorv1

Polynomial equation solver up to degree 2, written in C++11.

## Usage

```
./computor "equation"
```

Or just run `./computor` and type the equation when prompted.

## Examples

```
./computor "5 * X^0 + 4 * X^1 - 9.3 * X^2 = 1 * X^0"
./computor "5 * X^0 + 4 * X^1 = 4 * X^0"
./computor "4 * X^0 = 4 * X^0"
```

## What it does

Parses a polynomial equation, reduces it to the form `... = 0`, then solves it:

- **Degree 0** — either no solution or infinitely many
- **Degree 1** — one real solution
- **Degree 2** — two real solutions, one (double root), or two complex solutions depending on the discriminant

## Bonus

The bonus version adds:
- Intermediate steps shown for each solve
- Fractional output when possible (e.g. `3/4` instead of `0.75`)
- Simplified square roots for complex solutions (e.g. `3√2i/5`)
- More flexible input — supports `X`, `-X`, `3X`, `X^2` without requiring `*`

## Build

```
cd Mandatory
make
```

```
cd Bonus
make
```

Both produce a binary named `computor`.

## Supported input formats

Terms can be written as:
- `4.5 * X^2`
- `X^2`, `-X^2`, `3X^2`
- `X`, `-X`, `3X`
- `5` (constant)

Both `X` and `x` are accepted.
