#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

typedef struct
{
	struct CharString* ptr;
	size_t length;
	//size_t size;

	size_t(*ToSizeT)(void* ptr);
	long double (*ToDouble)(void* ptr);
	void (*PrintCharString)(void* ptr);
	void (*ClearCharString)(void* ptr);
	void (*DeleteCharString)(void* ptr);
	void* (*GetLine)(FILE* fp, size_t size, int c);

	char* data;
} CharString;



size_t _ToSizeT(const CharString* self)
{
	//convert to size_t
	return strtoull(self->data, self->data[self->length], 10);
}

long double _ToDouble(const CharString* self)
{
	//convert to size_t
	return strtold(self->data, self->data[self->length], 10);
}

void _PrintCharString(const CharString* self)
{
	printf("Length:%zu\nData:%s\nPointer:%p\n", self->length, self->data, self->ptr);
}

void _ClearCharString(CharString* self)
{
	if (!self || !(self->ptr = realloc(self->data, sizeof(char))))
	{
		fputs("Realloc was unable to reclaim memory!", stderr);
		exit(EXIT_FAILURE);
	}
	self->data[0] = '\0';
	self->length = 0;
}

void _DeleteCharString(CharString* self)
{
	self->ClearCharString(self);

	free(self);
}



typedef struct
{
	size_t id;
	CharString first;
	CharString last;

} Person;


inline CharString* _concat(CharString* lhs, CharString* rhs)
{
	size_t lhs_length = lhs->length;
	size_t rhs_length = rhs->length;
	size_t length = lhs_length + rhs_length;
	CharString* ret = NULL;
	ret = malloc(sizeof(CharString));
	if (!ret)
	{
		fputs("Malloc was unable to allocate more memory!", stderr);
		exit(EXIT_FAILURE);
	}

	ret->data = malloc(sizeof(char) * (length + 1));
	if (!ret->data)
	{
		fputs("Malloc was unable to allocate more memory!", stderr);
		exit(EXIT_FAILURE);
	}

	//Prefer assignment
	ret->data[length] = '\0';
	ret->length = length;

	memcpy(ret->data, lhs->data, lhs_length);
	memcpy(ret->data + lhs_length, rhs->data, rhs_length);
	ret->ptr = &ret;
	return ret;
}

inline CharString* _getLine(FILE* fp, size_t size, int c) {

	//Default value if none supplied
	c = (!c) ? '\n' : c;
	size = (size < 32) ? 32 : size;
	size_t buffer = size;

	CharString* ret = NULL;
	ret = malloc(sizeof(CharString));
	if (!ret)
	{
		fputs("Malloc was unable to allocate more memory!", stderr);
		exit(EXIT_FAILURE);
	}

	ret->data = malloc(sizeof(char) * buffer);
	if (!ret->data)
	{
		fputs("Malloc was unable to allocate more memory!", stderr);
		exit(EXIT_FAILURE);
	}

	int ch = 0;
	size_t len = 0;
	char* tmp = NULL;
	while (EOF != (ch = fgetc(fp))
		&& ch != c)
	{
		ret->data[len++] = ch;
		if (len == buffer)
		{
			tmp = realloc(ret->data, sizeof(char) * (buffer += size));
			if (!tmp)
			{
				fputs("Realloc was unable to allocate more memory!", stderr);
				exit(EXIT_FAILURE);
			}
			ret->data = tmp;
			tmp = NULL;
		}
	}

	//Prefer assignment
	ret->data[len] = '\0';
	ret->length = len;

	//Attempt to reclaim memory
	if (!realloc(ret->data, sizeof(char) * (len + 1)))
	{
		fputs("Realloc was unable to reclaim memory!", stderr);
		exit(EXIT_FAILURE);
	}
	ret->ptr = ret;
	free(tmp);


	ret->PrintCharString = _PrintCharString;//function pointer
	ret->ToSizeT = _ToSizeT;
	ret->ToDouble = _ToDouble;
	ret->ClearCharString = _ClearCharString;
	ret->DeleteCharString = _DeleteCharString;
	ret->GetLine = _getLine;

	return ret;
}


int main(void) {

	Person pp;



	pp.first = *_getLine(stdin, NULL, NULL);
	pp.last = *_getLine(stdin, NULL, NULL);
	pp.id = _ToSizeT(_getLine(stdin, NULL, NULL));

	printf("%p\n%p\n%p\n\n", pp.first.ptr, &pp.first, &pp);


	pp.first.DeleteCharString(pp.first.ptr);


	system("pause");
	return EXIT_SUCCESS;
}
