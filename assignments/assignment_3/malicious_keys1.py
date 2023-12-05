import itertools
import random

def generate_all_permutations(keys):
    first_4_chars = ''.join(set(keys[0][:4]))
    next_7_chars = ''.join(set(keys[0][4:11]))
    last_10_chars = ''.join(set(keys[0][-10:]))
    underscore = "_"

    all_first_4_permutations = list(itertools.permutations(first_4_chars))
    all_next_7_permutations = list(itertools.permutations(next_7_chars))
    all_last_10_permutations = list(itertools.permutations(last_10_chars))

    # Generate 10^5 cases intelligently chosen to cover all three types of permutations
    chosen_permutations = set()

    while len(chosen_permutations) < 5*(10**4):
        first_4_permutation = random.choice(all_first_4_permutations)
        next_7_permutation = random.choice(all_next_7_permutations)
        last_10_permutation = random.choice(all_last_10_permutations)

        permuted_key = ''.join(first_4_permutation) + ''.join(next_7_permutation) + underscore + ''.join(last_10_permutation)

        chosen_permutations.add(permuted_key)

    return list(chosen_permutations)

if __name__ == "__main__":
    keys = ["ABCD1234560_1234567890"]  # Add your keys here

    chosen_permutations = generate_all_permutations(keys)

    for i, permuted_key in enumerate(chosen_permutations):
        print('"'+permuted_key+'"'+',')
