# Database lib

## Constants

`DATABASE` is the absolut path of database file

## Functions

```c
int exec(char *sql)
```

Executes a SQL statement.

| Retorno | Significado |
| --- |  ---| 
| 0 | Operação realizada com sucesso |
| 1 | Ocorreu um erro na operação |

Para INSERT, UPDATE e DELETE: executa a função e imprime mensagens de sucesso ou erro.

Para SELECT, os dados são exibidos no terminal através da função de callback. Para alterar a forma como é mostrada, alterar essa função.
Cada linha retornada no SELECT passa pela função de callback.