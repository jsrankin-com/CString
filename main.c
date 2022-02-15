#include <stdio.h>
#include <stdlib.h>

typedef struct
{
	size_t length;
	char* data;
} CharString;

typedef struct
{
	unsigned id;
	CharString first;
	CharString last;

} Person;

inline CharString* _concat(CharString* lhs, CharString* rhs)
{
	size_t lhs_length = lhs->length;
	size_t rhs_length = rhs->length + 1;
	size_t size = lhs_length + rhs_length;
	CharString* ret = NULL;
	ret = malloc(sizeof(CharString));
	if (!ret)
	{
		fputs("Malloc was unable to allocate more memory!", stderr);
		return EXIT_FAILURE;
	}

	ret->data = malloc(sizeof(char) * size);
	if (!ret->data)
	{
		fputs("Malloc was unable to allocate more memory!", stderr);
		return EXIT_FAILURE;
	}

	memcpy(ret->data, lhs->data, lhs_length);
	memcpy(ret->data + lhs_length, rhs->data, rhs_length);

	return ret;


}

inline CharString* _getLine(FILE* fp, size_t size, int c) {

	if (!c)
		c = '\n';

	if (size < 32)
		size = 32;


	CharString* ret = NULL;
	ret = malloc(sizeof(CharString));
	if (!ret)
	{
		fputs("Malloc was unable to allocate more memory!", stderr);
		return EXIT_FAILURE;
	}

	ret->data = malloc(sizeof(char) * size);
	if (!ret->data)
	{
		fputs("Malloc was unable to allocate more memory!", stderr);
		return EXIT_FAILURE;
	}

	int ch = 0;
	size_t len = 0;
	char* tmp = NULL;
	while ((!EOF) || (EOF != (ch = fgetc(fp)) && ch != c))
	{
		ret->data[len++] = ch;
		if (len == size)
		{
			tmp = realloc(ret->data, sizeof(char) * (size += size));
			if (tmp)
			{
				ret->data = tmp;
				tmp = NULL;
			}
			else
			{
				fputs("Realloc was unable to allocate more memory!", stderr);
				return EXIT_FAILURE;
			}
		}
	}

	ret->data[len] = '\0';

	memcpy(&ret->length, &len, sizeof(size_t));
	ret->data = realloc(ret->data, sizeof(char) * len + 1);
	free(tmp);

	return ret;
}

int main(void) {
	CharString* str = NULL;
	Person* p = NULL;
	p = malloc(sizeof(Person));

	puts("PLEASE ENTER THE FIRST NAME");
	str = _getLine(stdin, NULL, NULL);
	memcpy(&p->first, str, sizeof(CharString));
	printf("%s\n", str->data);

	puts("PLEASE ENTER THE LAST NAME");
	str = _getLine(stdin, NULL, NULL);
	memcpy(&p->last, str, sizeof(CharString));

	p->id = 1;
	printf("%i %s %s\n", p->id, p->first.data, p->last.data);

	str = _concat(&p->first, &p->last);
	printf("CONCAT FIRST AND LAST: %s\n", str->data);
	free(str);

	printf("%i %s %s\n", p->id, p->first.data, p->last.data);

	system("pause");
	return EXIT_SUCCESS;
}