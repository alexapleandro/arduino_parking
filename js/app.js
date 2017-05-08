angular.module('app', ['app.controllers', 'app.factories', 'ngRoute', 'firebase'])

    .config(function ($routeProvider) {
        $routeProvider
            .when('/', {
                templateUrl: "views/home.html"
            })
            .when('/vagas', {
                templateUrl: "views/vagas.html",
                controller: 'listarController as vagas'
            })
            .when('/vaga/:id', {
                templateUrl: "views/vaga.html",
                controller: 'editarController as vaga'
            })
            .when('/novo', {
                templateUrl: "views/vaga.html",
                controller: 'addController as vaga'
            })
            .otherwise({
                redirectTo: '/'
            });

        var config = {
            apiKey: "AIzaSyCxpDMI9Vjb_vemXdK0kG6RwdtFxLobF2s",
            authDomain: "auto-parking-fcv.firebaseapp.com",
            databaseURL: "https://auto-parking-fcv.firebaseio.com",
            projectId: "auto-parking-fcv",
            storageBucket: "auto-parking-fcv.appspot.com",
            messagingSenderId: "322024687102"
        };
        firebase.initializeApp(config);
    });

