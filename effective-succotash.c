/* ♥ 2018 by Alexander Dahl <post@lespocky.de>.
 * Copying is an act of love. Please copy. */

#include <stdio.h>
#include <string.h>

#include <openssl/md5.h>

#define MYINPUT "foo"

int main( int argc, char *argv[] )
{
	unsigned char md5result[MD5_DIGEST_LENGTH];
	int i;

	(void) MD5( MYINPUT, strlen(MYINPUT), md5result );

	for ( i = 0; i < MD5_DIGEST_LENGTH; i++ )
	{
		(void) printf( "%02x", md5result[i] );
	}

	(void) printf( "\n" );

	return 0;
}
