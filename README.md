# Forward-Chaining-Inference-Engine
# Mini Expert System in C 
A small rule-based **expert system** written in C (UTBM — LO21 project).  
It manipulates logical propositions (true/false), a **knowledge base** (rules), a **fact base** (known true facts), and an **inference engine** that derives new facts by applying rules (forward chaining).

---

## Concept

- **Knowledge Base (KB)**: a list of rules.
- **Rule**: a *premise* (AND of propositions) and a *single conclusion*.
  - Example: `A AND B AND C => D`
  - Meaning: if the premise is true, we deduce that the conclusion is true.
- **Fact Base (FB)**: a list of propositions considered true.
- **Inference Engine**: iteratively applies rules whose premises are satisfied by the current facts to deduce new certain facts.

---

## Features

- Abstract Data Type **Rule**:
  - Create an empty rule
  - Append a proposition to the premise (tail insertion)
  - Set the conclusion
  - Recursively test if a proposition belongs to the premise
  - Remove a proposition from the premise
  - Check if the premise is empty
  - Access the head of the premise
  - Access the conclusion

- Abstract Data Type **Knowledge Base (KB)** as a list of rules:
  - Create an empty KB
  - Append a rule to the KB
  - Access the head rule

- **Forward-chaining inference engine**:
  - Starts from the initial fact base
  - Applies rules to deduce new facts
  - Stops when no new facts can be produced

---

## Example (Car Diagnosis)

**Rules**
- `¬reservoirVide AND pharesFonctionnent AND ¬moteurDemarre => problemeBougie`
- `¬moteurDemarre AND ¬pharesFonctionnent => problemeBatterie`
- `¬moteurDemarre AND pharesFonctionnent => problemeStarter`

**Initial facts**
- `¬moteurDemarre, pharesFonctionnent`

The engine deduces new facts such as `problemeStarter` (depending on the exact rule set and implementation details).

---


