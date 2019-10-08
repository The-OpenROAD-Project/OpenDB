///////////////////////////////////////////////////////////////////////////////
// BSD 3-Clause License
//
// Copyright (c) 2019, Nefelus Inc
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
// * Redistributions of source code must retain the above copyright notice, this
//   list of conditions and the following disclaimer.
//
// * Redistributions in binary form must reproduce the above copyright notice,
//   this list of conditions and the following disclaimer in the documentation
//   and/or other materials provided with the distribution.
//
// * Neither the name of the copyright holder nor the names of its
//   contributors may be used to endorse or promote products derived from
//   this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
// DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
// FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
// DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
// SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
// CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
// OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

#include <vector>
#include <string>
#include "../include/db.h"
#include "../dbCore.h"
#include "../dbNameServer.h"
#include "../dbStream.h"

USING_NAMESPACE_ADS;

int errors = 0;

void test_name_server( dbNameServer & server, const char * infile, const char * outfile )
{
    std::vector< std::string > strings;
    std::vector<uint> names;

    FILE * f = fopen( infile, "r" );

    if ( f == NULL )
    {
        fprintf( stderr, "Can not open file %s.\n", infile );
        ++errors;
        return;
    }

    // Add strings to server
    char line[8192];

    while( fgets( line, 8192, f ) )
    {
        line[strlen(line)-1] = '\0';
        std::string n(line);
        strings.push_back( n );
        uint id = server.getName( n.c_str() );
        assert( id );
        names.push_back( id );
    }

    fclose(f);

    // Compare strings to dbName's
    int i;

    for( i = 0; i < (int) strings.size(); ++i )
    {
        const std::string & n1 = strings[i];
        dbString n2;
        server.getString( names[i], n2 );

        if ( strcmp( n1.c_str(), n2.c_str() ) != 0 )
        {
            fprintf( stderr, "ERROR: string-server failed:\n" );
            ++errors;
        }
    }

    f = fopen(outfile, "w" );

    if ( f == NULL )
    {
        fprintf( stderr, "Can not open file %s.\n", outfile);
        ++errors;
        return;
    }

    // Save the server
    dbOStream dbout( NULL, f );
    dbout << server;
    fclose(f);
    
    f = fopen(outfile, "r" );

    if ( f == NULL )
    {
        fprintf( stderr, "Can not open file %s.\n", outfile);
        ++errors;
        return;
    }

    // Restore the server
    dbNameServer new_server(NULL);

    dbIStream dbin( NULL, f );
    dbin >> new_server;
    fclose(f);
    
    // Check the restored strings again
    for( i = 0; i < (int) strings.size(); ++i )
    {
        const std::string & n1 = strings[i];

        uint id  = new_server.findName( n1.c_str() );
        assert(id);
        
        dbString n2;
        new_server.getString( id, n2 );
 
        if ( strcmp( n1.c_str(), n2.c_str() ) != 0 )
        {
            fprintf( stderr, "ERROR: string-server failed:\n" );
            ++errors;
        }
    }
}

int main( int argc, char ** argv )
{
    if ( argc != 2 )
    {
        printf("usage test_strings data_file\n");
        exit(1);
    }

    dbNameServer flat(NULL);
    dbNameServer hier(NULL, '/');
    test_name_server( flat, argv[1], "name_server.flat" );
        //test_name_server( hier, argv[1], "name_server.hier" );
    flat.printStats();
    
    exit(errors);
}

