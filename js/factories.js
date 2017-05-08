angular.module('app.factories', [])


.factory('$bd_vagas', function($firebaseArray, $firebaseObject){
	var opcao = {};
	var ref = firebase.database().ref().child('vagas');
	var list = $firebaseArray(ref);
	var cache = [];

	opcao.listar = function(){	
		return list.$loaded().then(function(vagas){
			return cache = vagas;
		});
	};

	opcao.adicionar = function(vaga){
		return list.$add(vaga);
	};

	opcao.get = function(id){
		var vaga = cache.$getRecord(id);
		return vaga;
	};

	opcao.remover = function(id){
		var vaga = cache.$remove(list.$indexFor(id));
		return vaga;
	}

	opcao.editar = function(vaga){
		return list.$save(vaga);
	};

	return opcao;
})