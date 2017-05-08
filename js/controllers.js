angular.module('app.controllers', [])

    .controller('listarController', function ($scope, $location, $bd_vagas) {
        $scope.lista = [];

        $bd_vagas.listar().then(function (vagas) {
            $scope.lista = vagas;
        }, function (error) {
            console.log(error);
            alert(error.message);
        });

        $scope.remover = function (id) {
            $bd_vagas.remover(id).then(function () {
                //$location.path("/vagas");
                 alert('sucesso');
            }, function (error) {
                alert('Erro ao salvar');
                console.log(error);
            });
        };
    })

    .controller('addController', function ($scope, $location, $routeParams, $bd_vagas) {
        $scope.valor = {};
        $scope.salvar = function () {
            $bd_vagas.adicionar($scope.valor).then(function () {
                $location.path("/vagas");
            }, function (error) {
                alert('Erro');
                console.log(error);
            });
        };
    })

    .controller('editarController', function ($scope, $location, $routeParams, $bd_vagas) {

        var id = $routeParams.id;
        $scope.valor = $bd_vagas.get(id);
        $scope.salvar = function () {
            $bd_vagas.editar($scope.valor).then(function () {
                $location.path("/vagas");
            }, function (error) {
                alert('Erro ao salvar');
                console.log(error);
            });
        };

    });