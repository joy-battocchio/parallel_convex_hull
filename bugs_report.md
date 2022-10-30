Strange behavior:

```c
//in cloud_generator_testing(vector *v)
int x_test = 663;
int y_test = 8764;   
point test = (point){.x = x_test, .y = y_test}; 
v->pfVectorSet(v, 70, &test);

int x_test2 = 456;
int y_test2 = 777;
point test2 = (point){.x = x_test2, .y = y_test2};
v->pfVectorSet(v, 65, &test2);
```

```shell
point 64: ( 99 ; 198 )
point 65: ( 0 ; 99 )
point 66: ( 99 ; 198 )
point 67: ( 99 ; 198 )
point 68: ( 99 ; 198 )
point 69: ( 99 ; 198 )
point 70: ( 0 ; 8764 )
```

---------


