# Appendix B Provenance Log

This project intentionally uses a minimal amount of MINIX Appendix B-derived material.

## Policy
- Reuse only tiny low-level pieces required for early bring-up.
- Keep all reuse explicitly documented.
- Prefer small adaptations over large file copies.

## Derived items (V1)
1. File: include/minix_derived.h
   Origin: MINIX Appendix B, include/minix/const.h
   Derived symbols: CLICK_SIZE/CICK_SHIFT concept and TRUE/FALSE style constants.
   Adaptation: namespaced as MINIX_* and reduced to minimal subset.

2. File: kernel/memory.c
   Origin: MINIX Appendix B low-level copy/set routine behavior patterns (kernel assembly utility intent).
   Derived behavior: minimal freestanding memcpy/memset/memmove semantics used during early boot.
   Adaptation: rewritten in C with project-local types and interfaces.

3. File: kernel/main.c
   Origin: MINIX Appendix B, kernel/main.c startup organization.
   Derived behavior: staged kernel bring-up flow (early setup, kernel announce path, idle/scheduler placeholder).
   Adaptation: rewritten for this tiny freestanding VM OS and mapped to dual serial/VGA output.

## Verification checklist
- [ ] Confirm each origin against your local AppendixB.txt offsets/pages.
- [ ] Keep borrowed surface under the agreed minimum budget.
- [ ] Update this file for every additional borrowed snippet.
