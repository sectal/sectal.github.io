$(document).ready(function () {//code will run once the document is ready
    $('.menu-toggler').on('click',function(){//class menu toggler will appear when it is clicked
        $(this).toggleClass('open');//makes all transformations and movement start
        $('.top-nav').toggleClass('open');//makes top navigation appear
    });
    $('.top-nav').on('click',function(){//navigation will take you where you need to go
        $('.menu-toggler').removeClass('open');//menu toggler will be removed
        $('.top-nav').removeClass('open');//the navigation will disappear
    });
    $('nav a[href*="#"]').on('click',function(){//smooth scrolling for the navigation
        $('html,body').animate({
            scrollTop: $($(this).attr('href')).offset().top - 100
        },2000);
    });
    $('#up').on('click',function(){//class menu toggler will appear when it is clicked
        $('html,body').animate({
            scrollTop: 0
        },2000);
    });
    AOS.init({//fading and AOS start
        easing:'ease',
        duration:1800,
        once:true//animation only happens once
    });
});