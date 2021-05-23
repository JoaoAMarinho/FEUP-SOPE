echo 'TESTES A CORRER'

#sh test.sh NVEZES NTESTE

for i in $(seq 1 1 $1)
do
    sh tests/test_client.sh $2
done
