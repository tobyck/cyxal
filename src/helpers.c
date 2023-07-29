#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>
#include "helpers.h"

void vectorise(CyContext *ctx, CyElement element) {
	CyValueList *args = pop_args(ctx, element.arity_in);

	bool any_lists = false; // whether any of the args are lists or not

	for (int i = 0; i < element.arity_in; ++i) { // for each arg
		if (args->values[i]->type == ListType) { // if it's a list
			any_lists = true;
			break;
		}
	}

	if (any_lists) {
		size_t longest_arg_length = 0;

		for (int i = 0; i < args->size; ++i) {
			CyValue *arg = args->values[i];

			if (arg->type != ListType) continue; // ignore if not a list

			size_t arg_length = ((CyValueList *)arg->other)->size;

			if (arg_length > longest_arg_length) {
				longest_arg_length = arg_length;
			}
		}

		// turn all args into lists the length of the longest arg and push them to a new stack
		CyValueList *new_args[element.arity_in];

		for (int i = 0; i < args->size; ++i) {
			CyValue *arg = clone_cy_value(args->values[i]);

			if (arg->type == ListType) {
				new_args[i] = clone_cy_value_list(arg->other);
				while (new_args[i]->size < longest_arg_length) {
					push_cy_value(new_args[i], cy_value_new_num(L"0"));
				}
			} else {
				new_args[i] = empty_cy_value_list();
				for (int j = 0; j < longest_arg_length; ++j) {
					push_cy_value(new_args[i], clone_cy_value(arg));
				}
			}

			free_cy_value(arg);
		}

		CyValueList *result = empty_cy_value_list();

		for (int i = 0; i < longest_arg_length; ++i) {
			CyValueList *stack = push_empty_stack(ctx->stacks);
			for (int j = 0; j < element.arity_in; ++j) {
				push_cy_value(stack, clone_cy_value(new_args[j]->values[i]));
			}
			element.func(ctx);
			push_cy_value(result, pop_arg(ctx));
			pop_and_free_stack(ctx->stacks);
		}

		CyValue *result_value = cy_value_new_list(result);
		push_cy_value(last_stack(ctx->stacks), result_value);

		// free args that where cast to lists
		for (int i = 0; i < element.arity_in; ++i) {
			free_cy_value_list(new_args[i]);
		}
	} else {
		// push the args back
		for (int i = 0; i < args->size; ++i) {
			push_cy_value(last_stack(ctx->stacks), clone_cy_value(args->values[i]));
		}

		element.func(ctx);
	}

	free_cy_value_list(args);
}

/*CyValue *vectorise(CyContext *ctx, CyElement element, CyValueList *args) {
	bool no_lists = true;

	for (int i = 0; i < element.arity_in; ++i) {
		if (args->values[i]->type == ListType) {
			no_lists = false;
		}
	}

	if (no_lists) {
		// push a new stack to evaluate the element on
		push_empty_stack(ctx->stacks);

		// copy the args to the stack
		for (int i = 0; i < element.arity_in; ++i) {
			push_cy_value(last_stack(ctx->stacks), args->values[i]);
		}

		element.func(ctx); // run the element
		CyValue *result = pop_arg(ctx); // get the result
		pop_stack(ctx->stacks); // get rid of the temporary stack

		return result;
	}

	size_t longest_arg_length = 0; // set the longest to 0 initially

	for (int i = 0; i < element.arity_in; ++i) { // for each arg
		// if it's longer than the longest found so far
		if (args->values[i]->type == ListType && ((CyValueList *)args->values[i]->other)->size > longest_arg_length) {
			longest_arg_length = ((CyValueList *)args->values[i]->other)->size; // set it to the new longest
		}
	}

	for (int i = 0; i < element.arity_in; ++i) {
		// if the arg isn't a list, turn it into a list full of the value
		if (args->values[i]->type != ListType) {
			CyValue *singleton_list = cy_value_new_list(empty_cy_value_list());
			// fill it with the value to it's the length of the longest
			for (int _ = 0; _ < longest_arg_length; ++_) {
				push_cy_value(singleton_list->other, args->values[i]);
			}
			args->values[i] = singleton_list;
		} else { // if it is a list, fill it with 0s to the length of the longest
			if (((CyValueList *)args->values[i]->other)->size < longest_arg_length) {
				while (((CyValueList *)args->values[i]->other)->size < longest_arg_length) {
					push_cy_value(args->values[i]->other, cy_value_new_num(L"0"));
				}
			}
		}
	}

	CyValueList *result = empty_cy_value_list();

	for (int i = 0; i < longest_arg_length; ++i) {
		push_empty_stack(ctx->stacks);
		for (int j = 0; j < element.arity_in; ++j) {
			push_cy_value(last_stack(ctx->stacks), ((CyValueList *)args->values[j]->other)->values[i]);
		}
		element.func(ctx);
		push_cy_value(result, pop_arg(ctx));
		pop_stack(ctx->stacks);
	}

	// return the list as a CyValue
	return cy_value_new_list(result);
}*/

// checks if a string contains a certain char
bool contains(wchar_t *str, wchar_t chr) {
	if (wcschr(str, chr) != NULL) {
		return true;
	}
	return false;
}

wchar_t *append_str(wchar_t **dest, wchar_t *src) {
	*dest = realloc(*dest, (wcslen(*dest) + wcslen(src) + 1) * sizeof(wchar_t));
	wcscat(*dest, src);
	return *dest;
}

wchar_t *append_str_and_free(wchar_t **dest, wchar_t *src) {
	*dest = realloc(*dest, (wcslen(*dest) + wcslen(src) + 1) * sizeof(wchar_t));
	wcscat(*dest, src);
	free(src);
	return *dest;
}

// create a null-terminated string from a char
wchar_t *chr_to_wcs(wchar_t chr) {
	wchar_t *str = calloc(2, sizeof(wchar_t));
	str[0] = chr;
	return str;
}

// convert a standard string (char *) to a wide char string (4 bytes per char)
wchar_t *str_to_wcs(char *str) {
	size_t size = strlen(str) + 1; // + 1 for null byte
	wchar_t *wcs = malloc(size * sizeof(wchar_t));
	mbstowcs(wcs, str, size);
	return wcs;
}

// convert a wide char string to a normal string
char *wcs_to_str(wchar_t *wcs) {
	char *c = malloc(wcslen(wcs) + 1);
	sprintf(c, "%ls", wcs);
	return c;
}

// returns whether a char is a letter
bool is_letter(wchar_t wcs) {
	return (wcs >= L'a' && wcs <= L'z') || (wcs >= L'A' && wcs <= L'Z');
}