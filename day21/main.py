import re
import sys
from pathlib import Path
from typing import Dict, List, Set

with (Path(__file__).parent / sys.argv[1]).open() as f:
    lines = f.read().splitlines()

all_ingredients: List[str] = []
allerg2poss_ingr: Dict[str, Set[str]] = {}

for line in lines:
    match = re.fullmatch(r"([a-z ]*) \(contains ([a-z ,]*)\)", line)
    ingredients, allergens = match[1].split(), match[2].split(", ")
    all_ingredients.extend(ingredients)

    for allerg in allergens:
        if allerg not in allerg2poss_ingr:
            allerg2poss_ingr[allerg] = set(ingredients)
        else:
            allerg2poss_ingr[allerg] &= set(ingredients)

found_unambig_allerg = set()

while any(len(v) > 1 for v in allerg2poss_ingr.values()):
    unamb_allergen, corresponding_ingr = next(
        (k, next(iter(v)))
        for k, v in allerg2poss_ingr.items()
        if (len(v) == 1 and k not in found_unambig_allerg)
    )

    found_unambig_allerg.add(unamb_allergen)
    for k, v in allerg2poss_ingr.items():
        if k != unamb_allergen and corresponding_ingr in v:
            v.remove(corresponding_ingr)

allergenic_ingredients = {next(iter(v)) for v in allerg2poss_ingr.values()}

print(len([i for i in all_ingredients if i not in allergenic_ingredients]))

sorted_allergens = sorted(allerg2poss_ingr.keys())

print(",".join(next(iter(allerg2poss_ingr[k])) for k in sorted_allergens))
