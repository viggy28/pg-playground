# preload a shared library in Postgres

Postgres provides an option to preload a shared library (.so or .dylib) file. That file could contain a function or a type. This forms the foundation 
for writing Postgres extensions.

On Mac, cc points to Clang - LLVM's C Compiler

## Step 1: Compile and also provide the include directory for the headers
```
cc -I/usr/local/opt/postgresql@16/include/postgresql/server/ -c hello.c
```

## Step 2: Bundle and generate a shared object
```
cc -bundle -flat_namespace -undefined suppress -o hello.dylib hello.o
```

NOTE: On Mac, it's dylib (I don't think .so works)

## Step 3: Modify the shared_preload_library GUC
```
shared_preload_library='hello'
```

## Step 4: Copy the shared object file to Postgres pkglibdir
```
[23:44:54] viggy28:pg-playground git:(main*) $ pg_config --pkglibdir                         
/usr/local/opt/postgresql@16/lib/postgresql

cp hello.dylib /usr/local/opt/postgresql@16/lib/postgresql
```

## Step 5: Create the function
```
viggy28:eg-sh-preload-lib $ psql -d postgres -p 6432                                    
psql (16.2 (Homebrew))
Type "help" for help.
postgres=# create function hello() returns text as 'hello' language c; 
CREATE FUNCTION
```

## Step 6: Invoke the function
```
postgres=# select hello();
               hello                
------------------------------------
 Hello, world from shared library!!
(1 row)
```

### Reference 

https://www.postgresql.org/docs/current/xfunc-c.html#XFUNC-C-CODE

https://www.postgresql.org/docs/current/xfunc-c.html#XFUNC-C-DYNLOAD

https://www.postgresql.org/docs/current/xfunc-c.html#DFUNC

https://www.postgresql.org/docs/current/xfunc-c.html#XFUNC-C-V1-CALL-CONV