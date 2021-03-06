var renderer = '';
$(function(){
    var hotkeys = [16, 37, 38, 39, 40, 64, 67, 68, 65, 70, 71, 81, 83];
    $(document).ready(function() {
        $(document).keydown(function(event){
            if (hotkeys.indexOf(event.which) !== -1){
                $.post('/bomberman/keypress', {"key": event.which}, function(resp){
                    if (resp.status === "gameover"){
                        document.location = '/';
                    } else if (resp["data"]) {
                        // render interface
                        //var sampleData = '[[[0,0,0],[0,0,0],[0,0,255],[0,0,255],[0,0,255],[0,0,255],[0,0,0],[0,0,0],[0,0,0],[0,0,255],[0,0,255],[0,0,0],[0,0,0],[0,0,255],[0,0,255],[0,0,0],[0,0,0],[0,0,255],[0,0,0],[0,0,0],[0,0,0],[0,0,0],[0,0,255],[0,0,0],[0,0,0],[0,0,0],[0,0,0],[0,0,0],[0,0,0],[0,0,0],[0,0,255],[0,0,0],[0,0,0],[0,0,0],[0,0,0],[0,0,0],[0,0,255],[0,0,255],[0,0,255],[0,0,0],[0,0,0],[0,0,0],[0,0,255],[0,0,255],[0,0,255],[0,0,0],[0,0,0],[0,0,0],[0,0,0],[0,0,255],[0,0,255],[0,0,0],[0,0,0],[0,0,0],[0,0,0],[0,0,0],[0,0,0],[0,0,255],[0,0,255],[0,0,255],[0,0,255],[0,0,255],[0,0,255],[0,0,0]]]';
                        renderInterface(resp.data);
                    }
                }, 'json');
                return false;
            }
            return true;
        });

        renderInterface();

        function renderInterface(json) {
            for(var i = 0; i < 64; i++) {
                if (!$('#button'+i).length) {
                    var button = '<div class="color_button" id="button' + i + '"></div>';
                    $('#builder').append(button);
                }
                if (json[i] !== '') {
                    var tagColor = json[i];
                    $('#button'+i).css('background', 'rgb(' + tagColor[0] + ',' + tagColor[1] + ',' + tagColor[2] + ')');
                }
            }
        }
    });
});


