import random
import string

def generate_random_string(n, charset):
    return ''.join(random.choice(charset) for _ in range(n))

def generate_key():
    alphabet_charset = string.ascii_uppercase
    digit_charset = string.digits

    # Generate the prefix, middle, and suffix components
    prefix = generate_random_string(4, alphabet_charset)
    middle = generate_random_string(7, digit_charset)
    suffix = generate_random_string(10, digit_charset)

    # Create the key with the specified formatś
    key = f"{prefix}{middle}_{suffix}"

    return key

def generate_keys_with_single_difference(num_keys):
    keys = [generate_key()]

    while len(keys) < num_keys:
        prev_key = keys[-1]
        new_key = list(prev_key)

        # Generate a random index to change a character in the previous key
        index_to_change = random.randint(0, len(prev_key) - 1)

        # Determine the valid character set for the change based on the position
        if index_to_change < 4:
            valid_charset = string.ascii_uppercase
        elif index_to_change < 12:
            valid_charset = string.digits
        else:
            valid_charset = string.digits

        # Ensure that the underscore character is not changed
        if prev_key[index_to_change] != "_":
            # Generate a character different from the one at the chosen index
            new_char = random.choice([c for c in valid_charset if c != prev_key[index_to_change]])

            # Replace the character at the chosen index with the new character
            new_key[index_to_change] = new_char

            keys.append(''.join(new_key))

    return keys

if __name__ == "__main__":
    num_keys = 5*(10**4)  # Generate 100 keys

    keys = generate_keys_with_single_difference(num_keys)

    for key in keys:
        print('"'+key+'"'+',')
