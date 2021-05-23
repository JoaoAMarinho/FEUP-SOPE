# Sistemas Operativos
### MP2 - Application Server
----
[Enunciado](https://moodle.up.pt/pluginfile.php/205734/mod_resource/content/4/MP2-v3-25abr2021.pdf) do projeto está disponível na página do [Moodle](https://moodle.up.pt/course/view.php?id=1639) da unidade curricular.

### Cliente
* Todo o código referente ao `client` está na pasta client. Para aceder basta invocar o comando `cd client`.

#### Código Fonte e Relatório
O código fonte do projeto encontra-se na pasta `src`. O relatório está na pasta `doc` e é onde são abordados, com todo o detalhe, os pormenores de implementação. O código fornecido - com o executável do servidor encontra-se na pasta `mp21-code-v4`.
* Para compilar o código fonte do cliente é necessário executar o comando: `make client`.
* Para compilar o código fonte do servidor basta correr o comando: `make server`.

#### Testes
Depois de compilar todo o código necessário basta correr o seguinte commando: `sh tests/test_client.sh < 1 | 2 | 3 | clean >`.

### Server
* Todo o código referente ao `server` está na pasta server. Para aceder basta invocar o comando `cd server`.

#### Código Fonte e Relatório
O código fonte do projeto encontra-se na pasta `src`. O relatório está na pasta `doc` e é onde são abordados, com todo o detalhe, os pormenores de implementação. O código fornecido - com o executável do cliente encontra-se na pasta `mp22-code-v1`.
* Para compilar o código fonte do servidor basta correr o comando: `make server`.
* Para compilar o código fonte do cliente é necessário executar o comando: `make client`.

#### Testes
Depois de compilar todo o código necessário basta correr o seguinte commando: `sh tests/test_server.sh < 1 | 2 | 3 | clean >`.
