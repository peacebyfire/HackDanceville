$(function(){
    var hotkeys = [37, 38, 39, 40, 67, 81];
    $(document).keydown(function(event){
        console.log(event.which);
        if (hotkeys.indexOf(event.which) !== -1){
            $.post('/bomberman/keypress', {"key": event.which}, function(resp){
                if (resp === "gameover"){
                    alert("Game OVER");
                    document.location = '/';
                }
            });
            return false;
        }
        return true;
    });
});