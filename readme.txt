=== Aluno =============== RA ==
Cynthia Baran           135371
Guilherme Fontebasso    135973

Para a implementação de uma força, foi-se adicionado no Model mais um float,
que salva a informação da força que será feita. Isso foi necessário, pois para
se obter uma fórmula correta fisicamente, em Fisica::update, a força é somada
na fórmula de aceleração. Também foi necessário adicionar a força em Corpo::update,
pois a cada vez que ela é utilizada na fórmula, ela é zerada, como se fosse um breve
impulso.

Os botões + e - são utilizadas para acrescer e decrescer respectivamente, a força em 100 N.
W e S fazem a força nos corpos em sentidos opostos.