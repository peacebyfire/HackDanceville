$(function(){
    var hotkeys = [16, 37, 38, 39, 40, 64, 67, 68, 65, 70, 71, 81, 83];
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
