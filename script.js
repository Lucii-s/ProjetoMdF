function atualizarFormulario() {
  const opcao = document.querySelector('.caixa-opcoes').value;

//   const grupoDecript = document.querySelector('.grupo-decript'); 

  if (opcao === "opcao1") {
    const formChave = document.querySelector('#formChave');
    formChave.style.display = "grid";
    const formEncript = document.querySelector('#formEncript');
    formEncript.style.display = "none";
    const formDecript = document.querySelector('#formDecript');
    formDecript.style.display = "none";
    // grupoDecript.classList.add("escondido");


  } else if (opcao === "opcao2") {
    const formChave = document.querySelector('#formChave');
    formChave.style.display = "none";
    const formEncript = document.querySelector('#formEncript');
    formEncript.style.display = "grid";
    const formDecript = document.querySelector('#formDecript');
    formDecript.style.display = "none";
    grupoDecript.classList.add("escondido");

    // grupoDecript.classList.add("escondido");

  } else if (opcao === "opcao3") {
    const formChave = document.querySelector('#formChave');
    formChave.style.display = "none";
    const formEncript = document.querySelector('#formEncript');
    formEncript.style.display = "none";
    const formDecript = document.querySelector('#formDecript');
    formDecript.style.display = "grid";
    grupoDecript.classList.add("escondido");

    // grupoDecript.classList.remove("escondido");
  }
}

document.getElementById('formChave').addEventListener('submit', function(event) {
  event.preventDefault(); // Impede o envio do formulário normalmente
  const primo1 = document.getElementById('p').value;
  const primo2 = document.getElementById('q').value;
  const expoente = document.getElementById('e').value;
  createRSA().then((Module) => {
            const init_keys = Module.cwrap("init_keys", null, []);
            const set_public_keys = Module.cwrap("gerar_chave_publica", null, ["string", "string", "string"]);
            const get_public_n = Module.cwrap("get_public_n", "string", []);
            const get_public_e = Module.cwrap("get_public_e", "string", []);
            // const get_public_d = Module.cwrap("get_public_d", "string", []);
            // const free_mem = Module.cwrap("free_mem", null, ["number"]);

            // Inicializa
            init_keys();

            // Define chaves com valores como string
            set_public_keys(primo1, primo2, expoente);

            // Pega n como string
            const n = get_public_n();
            const e = get_public_e();
            // const d = get_public_d();

            const chavePubView = document.querySelector('#chavePub');
            chavePubView.textContent = e + " " + n;
        });
});


