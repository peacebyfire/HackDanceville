$(document).ready(function() {
    var model = {};
    var targetButton = '';
    var activeColor = '#000000';
    var mouseDown = false;
    var allowColor = false;
    var colorPicker = '';
    
    function buildBuilderInterface() {
        for(var i = 0; i < 64; i++) {
            var button = '<div class="color_button" id="button' + i + '"></div>';
            $('#builder').append(button);
            $('#button'+i).css('background', '#000000');
        }
        activateButtons();
    }

    function resetButtons() {
        for(var i = 0; i < 64; i++) {
            console.log(i);
            $('#button' + i).css('background', '#000000');
        }
    }

    function getPoints(tar) {
        var returnValue = new Array();
        $.each(tar.children('.frame_button'), function() {
            var self = this;
            var thisColor = self.style.backgroundColor;
            var rgb = thisColor.replace(/^rgba?\(|\s+|\)$/g,'').split(',');
            if (rgb[0] === ''){
                rgb = [255, 255, 255];
            }
            returnValue[returnValue.length] = $.map(rgb, function(el, i){ return parseInt(el);});
        });
        return returnValue;
    }

    function hexToRgb(hex) {
        var result = /^#?([a-f\d]{2})([a-f\d]{2})([a-f\d]{2})$/i.exec(hex);
        return result ? {
            r: parseInt(result[1], 16),
            g: parseInt(result[2], 16),
            b: parseInt(result[3], 16)
        } : null;
    }

    function handleColor(color) {
        activeColor = color;
        $('#colorValue').val(color);
        allowColor = false;
    } 

    /**
     * Bindings
     */
    function activateButtons() {
        $('.color_button').mousemove(function(e) {
            if (allowColor === true) {
                $('#' + e.currentTarget.id).css('background', activeColor);
            }
        });
        $('.color_button').mouseup(function() {
            if (allowColor === true) {
                allowColor = false;
            } else {
                allowColor = true;
            }
        });
        colorPicker = $.farbtastic('#color', handleColor);

        $('#colorValue').keyup(function() {
            colorPicker.setColor($('#colorValue').val());
        });
    }

    $('#saveFrame').mouseup(function() {
        allowColor = false;
        var count = $('#frames').children().length;
        $('#frames').prepend('<div id="frame'+(count+1)+'" class="displayFrame"><div class="deleteFrame">x</div></div>');
        $('.deleteFrame').mouseup(function() {
            $(this).parent().remove();
        });
        for(var i=0; i < 64; i++) {
            var button = $('#button'+i).clone();
            button.attr('id',"frame_button"+i);
            button.attr('class',"frame_button");
            $('#frame'+(count+1)).append(button);
        }
    });

    $('#clearButton').mouseup(function() {
        resetButtons();
    });

    $('#displayButton').mouseup(function() {
        var finalObject = new Array(),
            postData;
        $.each($('#frames').children(), function(e) {
            var data = getPoints($(this));
            finalObject[finalObject.length] = data;
        });
        postData = [{"name": "data", "value": JSON.stringify(finalObject.reverse())}];
        $.post( '/builder/display', postData, function() {
            $('#stopButton').show();
            console.log('done');
        }, 'json').fail(function() {
            console.log('failed');
        });
    });

    $('#stopButton').mouseup(function() {
        $.post('/builder/stop', function() {
            $('#stopButton').hide();
            console.log('stop display');
        });
    });

    buildBuilderInterface();
});
