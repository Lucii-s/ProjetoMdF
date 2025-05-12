function atualizarFormulario() {
  const opcao = document.querySelector('.caixa-opcoes').value;

  const tituloP = document.querySelector('.titulo-p');
  const tituloQ = document.querySelector('.titulo-q');
  const tituloE = document.querySelector('.titulo-e');

  const campoP = document.querySelector('.campo-p');
  const campoQ = document.querySelector('.campo-q');
  const campoE = document.querySelector('.campo-e');

  const grupoDecript = document.querySelector('.grupo-decript');

  if (opcao === "opcao1") {
    tituloP.innerText = "Digite o valor de p (número primo):";
    tituloQ.innerText = "Digite o valor de q (número primo):";
    tituloE.innerText = "Digite o valor de e (público):";

    campoP.placeholder = "Ex: 17";
    campoQ.placeholder = "Ex: 19";
    campoE.placeholder = "Ex: 7";

    grupoDecript.classList.add("escondido");

  } else if (opcao === "opcao2") {
    tituloP.innerText = "Digite sua mensagem a ser criptografada:";
    tituloQ.innerText = "Digite o valor de 'e' (chave pública):";
    tituloE.innerText = "Digite o valor de 'n' (chave pública):";

    campoP.placeholder = "Ex: Tralalero tralala";
    campoQ.placeholder = "Ex: 7";
    campoE.placeholder = "Ex: 323";

    grupoDecript.classList.add("escondido");

  } else if (opcao === "opcao3") {
    tituloP.innerText = "Digite o valor de p (número primo):";
    tituloQ.innerText = "Digite o valor de q (número primo):";
    tituloE.innerText = "Digite o valor de e (público):";

    campoP.placeholder = "Ex: 17";
    campoQ.placeholder = "Ex: 19";
    campoE.placeholder = "Ex: 7";

    grupoDecript.classList.remove("escondido");
  }
}
