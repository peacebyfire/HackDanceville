var renderer = '';
$(function(){
    var hotkeys = [16, 37, 38, 39, 40, 64, 67, 68, 65, 70, 71, 81, 83];
    $(document).ready(function() {
        $(document).keydown(function(event){
            if (hotkeys.indexOf(event.which) !== -1){
                $.post('/bomberman/keypress', {"key": event.which}, function(resp){
                    if (resp === "gameover"){
                        alert("Game OVER");
                        document.location = '/';
                    } else {
                        // render interface
                        //var sampleData = '[[[0,0,0],[0,0,0],[0,0,255],[0,0,255],[0,0,255],[0,0,255],[0,0,0],[0,0,0],[0,0,0],[0,0,255],[0,0,255],[0,0,0],[0,0,0],[0,0,255],[0,0,255],[0,0,0],[0,0,0],[0,0,255],[0,0,0],[0,0,0],[0,0,0],[0,0,0],[0,0,255],[0,0,0],[0,0,0],[0,0,0],[0,0,0],[0,0,0],[0,0,0],[0,0,0],[0,0,255],[0,0,0],[0,0,0],[0,0,0],[0,0,0],[0,0,0],[0,0,255],[0,0,255],[0,0,255],[0,0,0],[0,0,0],[0,0,0],[0,0,255],[0,0,255],[0,0,255],[0,0,0],[0,0,0],[0,0,0],[0,0,0],[0,0,255],[0,0,255],[0,0,0],[0,0,0],[0,0,0],[0,0,0],[0,0,0],[0,0,0],[0,0,255],[0,0,255],[0,0,255],[0,0,255],[0,0,255],[0,0,255],[0,0,0]]]';
                        renderInterface(resp);
                    }
                });
                return false;
            }
            return true;
        });

        renderInterface();

        function renderInterface(sampleData) {
            json = '';
            if (sampleData && sampleData !== '') {
                json = $.parseJSON(sampleData);
            }
            for(var i = 0; i < 64; i++) {
                if (!$('#button'+i).length) {
                    var button = '<div class="color_button" id="button' + i + '"></div>';
                    $('#builder').append(button);
                }
                if (sampleData && json[i] !== '') {
                    var tagColor = json[0][i];
                    $('#button'+i).css('background', 'rgb(' + tagColor[0] + ',' + tagColor[1] + ',' + tagColor[2] + ')');
                }
            }
        }
    });
});


