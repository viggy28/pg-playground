
```
cc -I/usr/local/opt/postgresql@16/include/postgresql/server/ -I/usr/local/opt/postgresql@16/include/postgresql/server/access -c xact_cb.c
```

```
c -bundle -flat_namespace -undefined suppress -o xact_cb.dylib xact_cb.o
```