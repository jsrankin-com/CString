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
	//Includes \0
	size_t rhs_length = rhs->length + 1;
	size_t size = lhs_length + rhs_length;
	CharString* ret = NULL;
	ret = malloc(sizeof(CharString));
	if (!ret)
	{
		fputs("Malloc was unable to allocate more memory!", stderr);
		return EXIT_FAILURE;
	}

	ret->data = malloc(sizeof(char)*size);
	if (!ret->data)
	{
		fputs("Malloc was unable to allocate more memory!", stderr);
		return EXIT_FAILURE;
	}

	memcpy(ret->data, lhs->data, lhs_length);
	memcpy(ret->data + lhs_length, rhs->data, rhs_length);

	return ret;
}

inline int _compare(CharString* lhs, CharString* rhs)
{
	if (lhs->length != rhs->length)
	{
		return 1;
	}

	for (size_t i = 0; i < lhs->length; ++i)
	{
		if (lhs->data[i] != rhs->data[i])
		{
			return 1;
		}
	}

	return 0;
}

inline int _toInt(CharString* str)
{
	int ret = strtol(str->data, NULL, 10);

	return ret;
}

inline int _toDouble(CharString* str)
{
	int ret = strtod(str->data, NULL);

	return ret;
}

inline CharString* _getLine(FILE* fp, size_t size, int c) {

	if (!c)
		c = '\n';

	//Assuming 32 bit system
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
			tmp = realloc(ret->data, sizeof(char)*(size += size));
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
	free(tmp);
	ret->data[len] = '\0';

	ret->length = len;
	ret->data = realloc(ret->data, sizeof(char)*len+1);
	
	return ret;
}

int main(void) {
	CharString* str = NULL;
	Person* p = NULL;
	p = malloc(sizeof(Person));

	puts("PLEASE ENTER THE FIRST NAME");
	str = _getLine(stdin, NULL, NULL);
	memcpy(&p->first, str, sizeof(CharString));
	printf("data:%s\nlength:%i\n\n", str->data, str->length);

	puts("PLEASE ENTER THE LAST NAME");
	str = _getLine(stdin, NULL, NULL);
	memcpy(&p->last, str, sizeof(CharString));
	printf("data:%s\nlength:%i\n\n", str->data, str->length);

	puts("PLEASE ENTER THE PERSON ID");
	str = _getLine(stdin, NULL, NULL);
	p->id = _toInt(str);

	printf("%i %s %s\n", p->id, p->first.data, p->last.data);

	str = _concat(&p->first, &p->last);
	printf("CONCAT FIRST AND LAST: %s\n", str->data);
	free(str);

	if (_compare(&p->first, &p->last))
	{ 
		printf("COMPARE DETERMINED THE NAMES WERE DIFFERENT\n"); 
	}
	free(p);
	system("pause");
	return EXIT_SUCCESS;
}
