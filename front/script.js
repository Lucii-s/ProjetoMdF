function atualizarFormulario() {
  const opcao = document.querySelector('.caixa-opcoes').value;

  const tituloP = document.querySelector('.titulo-p');
  const tituloQ = document.querySelector('.titulo-q');
  const tituloE = document.querySelector('.titulo-e');
  const tituloDecript = document.querySelector('.titulo-decript');

  const campoP = document.querySelector('.campo-p');
  const campoQ = document.querySelector('.campo-q');
  const campoE = document.querySelector('.campo-e');
  const campoDecript = document.querySelector('.campo-decript');

  // Esconde os campos de descriptografia por padrão
  tituloDecript.classList.add('escondido');
  campoDecript.classList.add('escondido');

  if (opcao === "opcao1") {
    tituloP.innerText = "Digite o valor de p (número primo):";
    tituloQ.innerText = "Digite o valor de q (número primo):";
    tituloE.innerText = "Digite o valor de e (público):";

    campoP.placeholder = "Ex: 17";
    campoQ.placeholder = "Ex: 19";
    campoE.placeholder = "Ex: 7";
  } else if (opcao === "opcao2") {
    tituloP.innerText = "Digite sua mensagem a ser criptografada:";
    tituloQ.innerText = "Digite o valor de 'e' (chave pública):";
    tituloE.innerText = "Digite o valor de 'n' (chave pública):";

    campoP.placeholder = "Ex: Tralalero tralala";
    campoQ.placeholder = "Ex: 7";
    campoE.placeholder = "Ex: 323";
  } else if (opcao === "opcao3") {
    tituloP.innerText = "Digite o valor de p (número primo):";
    tituloQ.innerText = "Digite o valor de q (número primo):";
    tituloE.innerText = "Digite o valor de e (público):";

    campoP.placeholder = "Ex: 17";
    campoQ.placeholder = "Ex: 19";
    campoE.placeholder = "Ex: 7";

    // Mostra os campos de descriptografia
    tituloDecript.classList.remove('escondido');
    campoDecript.classList.remove('escondido');
  }
}
