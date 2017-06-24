#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *derived_lookup_table(char *);
int contains(char *, char);

int main(void) {

	char *s = "attack";

	printf("The initial word is: %s\n", s);

	char *table = derived_lookup_table(s);

	printf("Derived lookup table: %s\n\n", table);

	// We need to free table at the end in order to avoid a (small) memory leak
	// This is because table is actually a pointer the the memory we calloced and
	// then later realloced in derived_lookup_table.

	// Change 1: Inserted: free(table);
	free(table);

	return EXIT_SUCCESS;
}

char *derived_lookup_table(char *s) {
	// This used to malloc(sizeof(s)), which only gives the space for a pointer, not
	// the full string that we want. Indeed, for the string "attack", the 8 bytes
	// allocated by the original malloc is sufficient, but for strings longer than
	// 7 characters (excluding the terminating '\0'), we would run out of space.

	// We should also initalise this string to '\0' characters, since contains
	// requires to know the current length of o_out (so we use calloc instead).
	// Now, as long as we write no more than strlen(s) characters to o_out, we
	// will always have a valid string that ends exactly where it should. Hence
	// strlen will give the correct value when called in contains.

	// Change 2: Used to be: char *o_out = malloc(sizeof(s));
	//                  Now: char *o_out = calloc(strlen(s) + 1, sizeof(char));
	char *o_out = calloc(strlen(s) + 1, sizeof(char));

	if (o_out == NULL) {
		perror("malloc");
		exit(EXIT_FAILURE);
	}

	// We don't need or want a second malloc here (for o_end), since we're just
	// pointing o_end to the same (already allocated) memory location as o_out.

	// Change 3: Removed: char *o_end = malloc(sizeof(s));
	char *o_end = o_out;

	for ( ; *s != '\0'; ++s) {
		if (!contains(o_out, *s)) {
			*o_end++ = *s;
		}
	}

	// This needs to point to the beginning of our lookup table, so first param
	// should be o_out instead of out (which is an uninitialised value).

	// Change 4: Used to be: char *out = realloc(out, o_end, o_out + 1);
	//                  Now: char *out = realloc(o_out, o_end - o_out + 1);
	char *out = realloc(o_out, o_end - o_out + 1);

	if (out == NULL) {
		perror("realloc");
		exit(EXIT_FAILURE);
	}

	return out;
}

int contains(char *s, char c) {

	for (int i = 0; i < strlen(s); i++) {
		if (s[i] == c) {
			return 1;
		}
	}

	return 0;
}