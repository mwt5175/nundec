/************************************************************************
*
*	main.c - Symbolic Name Undecorator for C and C++ symbols.
*
************************************************************************/

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include <math.h>

/*
Symbolic names have the following EBNF for compatability with Visual Studio

<public name> ::= ? <name> @ [<namespace> @]* @ 3 <type> <storage class>
<public name> ::= ? <name> @ [<class name> @]* @ 2 <type> <storage class>

Global function:
<public name> ::= ? <function name> @ [<namespace> @]* @ <near far>
	<calling conv> [<stor ret>] <return type> [<parameter type>]*  <term> Z

Class member function:
<public name> ::= ? <function name> @ [<class name> @]* @ <modif> [<const vol>]
	<calling conv> [<stor ret>] <return type> [<parameter type>]*  <term> Z
*/

//! Pointer to current token
char*   _currentTok = 0;

/**
* Goes to next character
* \return 0: end of characters, 1: success
*/
int getNext () {

	if (*_currentTok) {
		_currentTok++;
		return 1;
	}
	return 0;
}

/**
* Undecorate number code
* \return 0: error, 1: success
*/
int undecNumberMSVC () {

	int value = 0;
	//
	// If digit, value - digit - 1
	//
	if (isdigit (*_currentTok)) {
		int digit = *_currentTok - '0';
		printf ("%i", digit+1);
		return 1;
	}
	//
	// If A-P, its a hex number. Can be multiple digits, end in @
	//
	if (*_currentTok >= 'A' && *_currentTok <= 'P') {
		//
		// Get digit count so we know what to raise power to
		//
		int power = 0;
		double convertValue = 0.0;
		while (_currentTok [power] != '@')
			power++;
		power--;
		//
		// Convert each digit into a single hex number
		//
		while (*_currentTok != '@') {
			int digit = *_currentTok - 'A';
			convertValue += digit * pow (16, power);
			power--;
			getNext ();
		}
		value = (int)convertValue;
		printf ("%i", value);
		return 1;
	}
	//
	// If A@ value is 0
	//
	if (*_currentTok == 'A' && _currentTok [1] == '@') {
		printf ("0");
		return 1;
	}
	//
	// If ? value is -N coded as above
	//
	if (*_currentTok == '?') {
		getNext();
		// If A-P, its a hex number. Can be multiple digits, end in @
		if (*_currentTok >= 'A' && *_currentTok <= 'P') {
			//
			// Get digit count so we know what to raise power to
			//
			int power = 0;
			double convertValue = 0.0;
			while (_currentTok [power] != '@')
				power++;
			power--;
			//
			// Convert each digit into a single hex number
			//
			while (*_currentTok != '@') {
				int digit = *_currentTok - 'A';
				convertValue += digit * pow (16, power);
				power--;
				getNext ();
			}
			value = (int)convertValue;
			value -= value;
			printf ("%i", value);
			return 1;
		}
	}
	return 0;
}

/**
* Undecorate operator function code
* \return 0: error, 1: success
*/
int undecOperCodeMSVC () {

	char* functionName = 0;

	if (_currentTok [0] != '?')
		return 0;
	if (!getNext())
		return 0;
	switch (_currentTok [1]) {
		case '2': functionName = "operator new"; break;
		case '3': functionName = "operator delete"; break;
		case '4': functionName = "operator="; break;
		case '5': functionName = "operator>>"; break;
		case '6': functionName = "operator<<"; break;
		case '7': functionName = "operator!"; break;
		case '8': functionName = "operator=="; break;
		case '9': functionName = "operator!="; break;
		case 'A': functionName = "operator[]"; break;
		case 'B': functionName = "operator "; break;
		case 'C': functionName = "operator->"; break;
		case 'D': functionName = "operator*"; break;
		case 'E': functionName = "operator++"; break;
		case 'F': functionName = "operator--"; break;
		case 'G': functionName = "operator-"; break;
		case 'H': functionName = "operator+"; break;
		case 'I': functionName = "operator&"; break;
		case 'J': functionName = "operator->*"; break;
		case 'K': functionName = "operator/"; break;
		case 'L': functionName = "operator%"; break;
		case 'M': functionName = "operator<"; break;
		case 'N': functionName = "operator<="; break;
		case 'O': functionName = "operator>"; break;
		case 'P': functionName = "operator>="; break;
		case 'Q': functionName = "operator,"; break;
		case 'R': functionName = "operator()"; break;
		case 'S': functionName = "operator~"; break;
		case 'T': functionName = "operator^"; break;
		case 'U': functionName = "operator|"; break;
		case 'V': functionName = "operator&&"; break;
		case 'W': functionName = "operator||"; break;
		case 'X': functionName = "operator*="; break;
		case 'Y': functionName = "operator+="; break;
		case 'Z': functionName = "operator-="; break;
		case '_': {
			if (!getNext())
				return 0;
			switch (_currentTok [2]) {
				// case '':
				case '\0': functionName = "operator/="; break;
				case '1': functionName = "operator%="; break;
				case '2': functionName = "operator>>="; break;
				case '3': functionName = "operator<<="; break;
				case '4': functionName = "operator&="; break;
				case '5': functionName = "operator|="; break;
				case '6': functionName = "operator^="; break;
				case '7': functionName = "\'vftable\'"; break;
				case '8': functionName = "\'vbtable\'"; break;
				case '9': functionName = "\'vcall\'"; break;
				case 'A': functionName = "\'typeof\'"; break;
				case 'B': functionName = "\'local static guard\'"; break;
				case 'C': functionName = "\'string\'"; break;
				case 'D': functionName = "\'vbase destructor\'"; break;
				case 'E': functionName = "\'vector deleting destructor\'"; break;
				case 'F': functionName = "\'default constructor closure\'"; break;
				case 'G': functionName = "\'scalar deleting destructor\'"; break;
				case 'H': functionName = "\'vector constructor iterator\'"; break;
				case 'I': functionName = "\'vector destructor iterator\'"; break;
				case 'J': functionName = "\'vector vbase constructor iterator\'"; break;
				case 'K': functionName = "\'virtual displacement map\'"; break;
				case 'L': functionName = "\'eh vector constructor iterator\'"; break;
				case 'M': functionName = "\'eh vector destructor iterator\'"; break;
				case 'N': functionName = "\'eh vector vbase constructor iterator\'"; break;
				case 'O': functionName = "\'copy constructor closure\'"; break;
				case 'R': {
				//
				// Need to find how to handle this
				//
					break;
				}
				case 'S': functionName = "\'local vftable\'"; break;
				case 'T': functionName = "\'local vftable constructor closure\'"; break;
				case 'U': functionName = "operator new[]"; break;
				case 'V': functionName = "operator delete[]"; break;
				case 'X': functionName = "\'placement delete closure\'"; break;
				case 'Y': functionName = "\'placement delete[] closure\'"; break;
			};
			break;
		}
	};

	if (functionName)
		printf (" %s", functionName);
	return 1;
}

/**
* Converts storage class code to string
* \return 0: error, 1: success
*/
int storageClassMSVC () {

	char* storageClass = 0;
	switch (*_currentTok) {
		case 'A': storageClass = 0; break; //"near"
		case 'B': storageClass = "const"; break;
		case 'C': storageClass = "volatile"; break;
		case 'D': storageClass = "const volatile"; break;
		case 'E': storageClass = "far"; break;
		case 'F': storageClass = "const far"; break;
		case 'G': storageClass = "volatile far"; break;
		case 'H': storageClass = "const volatile far"; break;
		case 'I': storageClass = "huge"; break;
	};

	if (storageClass) {
		printf (" %s", storageClass);
		return 1;
	}
	return 0;
}

/**
* Converts data type code to string
* \return 0: error, 1: success
*/
int dataTypeMSVC () {

	char* dataType = 0;

	switch (*_currentTok) {
		case 'X': dataType = "void"; break;
		case 'D': dataType = "char"; break;
		case 'C': dataType = "signed char"; break;
		case 'E': dataType = "unsigned char"; break;
		case 'F': dataType = "short int"; break;
		case 'G': dataType = "unsigned short int"; break;
		case 'H': dataType = "int"; break;
		case 'I': dataType = "unsigned int"; break;
		case 'J': dataType = "long int"; break;
		case 'K': dataType = "unsigned long int"; break;
		case 'M': dataType = "float"; break;
		case 'N': dataType = "double"; break;
		case 'O': dataType = "long double"; break;
		case 'Z': dataType = "..."; break;
		case '_': {
			if (!getNext())
				return 0;
			switch (*_currentTok) {
				case 'N': dataType = "bool"; break;
				case 'J': dataType = "__int64"; break;
				case 'K': dataType = "unsigned __int64"; break;
				case 'W': dataType = "wchar_t"; break;
			};
			break;
		}
		case 'P': {
			//
			// Pointer types follow the form P[E][A]X
			// where E is either 'M', 'E' or not there
			// and 'A' is data type code
			//
			if (!getNext())
				return 0;
			switch (*_currentTok) {
				case 'M': {
					printf (" __based()"); //64 bit
					if (!getNext())
						return 0;
					storageClassMSVC ();
					getNext();
					break;
				}
				case 'E': {
					printf (" __ptr64");
					if (!getNext())
						return 0;
					storageClassMSVC ();
					getNext();
					break;
				}
			};
			storageClassMSVC ();
			getNext();
			if (*_currentTok == 'Y') {
				//
				// This is an array
				// First number tells us number of dimensions
				//
				int count = 0;
				getNext();
				if (!isdigit (*_currentTok))
					return 0;
				count = (*_currentTok - '0') + 1;
				//
				// Now get array dimensions
				//
				getNext();
				while (count--) {
					printf (" [");
					if (!undecNumberMSVC())
						break;
					printf ("]");
					getNext();
				}
			}
			if (!dataTypeMSVC ())
				return 1;
			printf (" *");
			break;
		}
	};

	if (dataType)
		printf (" %s", dataType);
	return 1;
}

/**
* Undecorate calling convention code
* \return 0: error, 1: success
*/
int callingConventionMSVC () {

	char* convention = 0;
	switch (*_currentTok) {
		case 'A': convention = "__cdecl"; break;    //interrupt
		case 'C': convention = "__pascal"; break;   //__fortran
		case 'E': convention = "__thiscall"; break;
		case 'G': convention = "__stdcall"; break;
		case 'I': convention = "__fastcall"; break;
	};
	if (convention) {
		printf (" %s", convention);
		return 1;
	}
	return 0;
}

/**
* Undecorate function distance code
* \return 0: error, 1: success
*/
int functionDistanceMSVC () {

	if (*_currentTok == 'Y' || *_currentTok == 'Q') {
		printf (" near");
		return 1;
	}
	else
	if (*_currentTok == 'Z' || *_currentTok == 'R') {
		printf (" far");
		return 1;
	}
	return 0;
}

/**
* Undecorate function return storage class code
* \return 0: error, 1: success
*/
int returnStorageClassMSVC () {

	char* storageClass = 0;

	if (*_currentTok != '?')
		return 0;
	switch (_currentTok[1]) {
		case 'A': storageClass = 0; break;
		case 'B': storageClass = "const"; break;
		case 'C': storageClass = "volatile"; break;
		case 'D': storageClass = "const volatile"; break;
	};
	if (storageClass) {
		getNext();
		printf (" %s", storageClass);
		return 1;
	}
	return 0;
}

/**
* Undecorate public function symbol
* \return 0: error, 1: success
*/
int publicFunctionMSVC () {

// Public
//<public name> ::= ? <function name> @ [<namespace> @]* @ <near far>
//	     <calling conv> [<stor ret>] <return type> [<parameter type>]*  <term> Z
// Class Member
//<public name> ::= ? <function name> @ [<class name> @]* @ <modif> [<const vol>]
//	     <calling conv> [<stor ret>] <return type> [<parameter type>]*  <term> Z

	char name[64];
	unsigned int i=0;
	//
	// ? <function name> @
	//
	if (*_currentTok != '?')
		return 0;
	memset (name, 0, 64);
	while (getNext()) {
		if (!isalnum(*_currentTok))
			break;
		name[i++] = *_currentTok;
	}
	if (*_currentTok != '@')
		return 0;
	getNext();
	//
	// [<namespace> @]* @ or [<class name> @]* @
	//
	while (getNext()) {
		char buf [64];
		memset (buf, 0, 64);
		if (!isalnum(*_currentTok))
			break;
		buf[i++] = *_currentTok;
		printf ("%s::");
	}
	if (*_currentTok != '@')
		return 0;
	getNext();
	//
	// <near far> or <modif> [<const vol>]
	//
	if (!functionDistanceMSVC ()) {
		// call  <modif> [<const vol>] instead
		printf ("class member function!");
	}
	getNext ();
	//
	// <calling conv> [<stor ret>] <return type> [<parameter type>]*  <term> Z
	//
	if (!callingConventionMSVC ())
		return 0;
	getNext();
	if (returnStorageClassMSVC())
		getNext();
	dataTypeMSVC ();
	getNext();
	//
	//<term> is @ except if the parameter list is void (X) or ends with ... (Z).
	//In these cases, the @ is omitted because the list is sure to end here
	//
	printf (" %s (", name);
	while (*_currentTok != 'Z' && *_currentTok != 'X' && *_currentTok != '@') {
		dataTypeMSVC();
		getNext();
		if (_currentTok[1] != 'Z' && *_currentTok != 'X' && *_currentTok != '@')
			printf (",");
	}
	printf (")\n\r");
	return 1;
}

/**
* Undecorate public name symbol
* \return 0: error, 1: success
*/
int publicNameMSVC () {
	// <public name> ::= ? <name> @ [<namespace> @]* @ 3 <type> <storage class>
	// <public name> ::= ? <name> @ [<class name> @]* @ 2 <type> <storage class>

	char name[64];
	unsigned int i=0;
	//
	// ? <name> @
	//
	if (*_currentTok != '?')
		return 0;
	memset (name, 0, 64);
	while (getNext()) {
		if (!isalnum(*_currentTok))
			break;
		name[i++] = *_currentTok;
	}
	if (*_currentTok != '@')
		return 0;
	//
	// [<namespace> @]*
	//
	while (1) {
		char buf [64];
		int i = 0;
		memset (buf, 0, 64);
		while (getNext()) {
			if (!isalnum(*_currentTok))
				break;
			buf[i++] = *_currentTok;
		}
		printf ("%s::", buf);
		if (*_currentTok != '@')
			return 0;
		if (_currentTok[1] == '@')
			break;
	}
	//
	//@ 3 <type> <storage class>
	//@ 2 <type> <storage class>
	//
	getNext();
	if (*_currentTok != '@')
		return 0;
	getNext();
	if (*_currentTok != '2' && *_currentTok != '3')
		return 0;
	if (*_currentTok == '2')
		printf (" public: static"); //static class member object
	getNext();
	dataTypeMSVC ();
	getNext();
	storageClassMSVC ();
	printf (" %s\n\r", name);
	return 1;
}

/**
* Undecorate symbol
* \param   str    Symbolic name
*/
void undecSymbol (char* str) {
	//
	// C style
	//
	_currentTok = str;
	if (*_currentTok == '_') {
		printf ("%s", &_currentTok [1]);
		return;
	}
	//
	// C++ style
	//
	if (_currentTok [strlen (_currentTok)-1] == 'Z')
		publicFunctionMSVC ();
	else
		publicNameMSVC ();
}

/**
* Entry point
* \param   argc   Operand count
* \param   argv   Array of operands
* \return Error code
*/
int main (int argc, char** argv) {

	if (argc!=2) {
		printf ("Syntax: nundec symbol");
		return EXIT_FAILURE;
	}
	undecSymbol (argv[1]);
	return EXIT_SUCCESS;
}
