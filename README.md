# DPLL

This repository contains my implementation of the Davis–Putnam–Logemann–Loveland (DPLL) algorithm, which is used to decide the satisfiability of propositional logic formulas in Conjunctive Normal Form (CNF).

The program uses the DPLL algorithm on a knowledge base made up of satisfiable sentences, to generate a model whose truth values are a solution to the given problem.

The knowledge base for the given problem is represented in marbles.cnf and is explained below:
  - You have 3 buckets.
  - Each bucket contains either red, green, or both red and green marbles.
  - Each bucket has an incorrect label on it (if the label is red, then the bucket can't contain only red marbles).
  - You have drawn a red marble from the first bucket, a green marble from the second bucket, and a red marble from the third bucket.
  - The first bucket has a green label, the second bucket has a red label, and the third bucket has a both label.

The program uses DPLL and marbles.cnf to generate a complete model for the problem where the truth values will show which bucket contains which color of marbles.

#Heuristic

In order to make DPLL more effiecient, I used the Unit-Clause heuristic. 
