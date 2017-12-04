<!DOCTYPE html>
<html>
    <head>
        <title>Statistics</title>
        <!-- Bootstrap -->
        <link href="bootstrap/css/bootstrap.min.css" rel="stylesheet" media="screen">
        <link href="bootstrap/css/bootstrap-responsive.min.css" rel="stylesheet" media="screen">
        <link href="assets/styles.css" rel="stylesheet" media="screen">
        <script src="vendors/modernizr-2.6.2-respond-1.1.0.min.js"></script>
        <script type="text/javascript">
            /*window.onload = function(){
                var name = $('#plantSelect').val();
                alert(name+"분석 페이지 로딩완료");
            }*/
        </script>
    </head>
    
    <body>
        <div class="navbar navbar-fixed-top">
            <div class="navbar-inner">
                <div class="container-fluid" style="margin-left: 4em">
                    <a class="btn btn-navbar" data-toggle="collapse" data-target=".nav-collapse"> <span class="icon-bar"></span>
                     <span class="icon-bar"></span>
                     <span class="icon-bar"></span>
                    </a>
                    <a class="brand" href="#">Let's Grow</a>
                    <!--/.nav-collapse -->
                </div>
            </div>
        </div>
        <div class="container-fluid">
            <div class="row-fluid">
                <div class="span3" id="sidebar">
                    <ul class="nav nav-list bs-docs-sidenav nav-collapse collapse">
                        <li class="active">
                            <a href="stats.html"><i class="icon-chevron-right"></i> 적합도 시스템</a>
                        </li>
                        <li>
                            <a href="index.html"><i class="icon-chevron-right"></i> 추천 시스템</a>
                        </li>
                    </ul>
                </div>

                <!--/span-->
                <div class="span9" id="content">
                      <!-- morris stacked chart -->
                    <div class="row">
                        <div class="span12">
                            <div class="btn-group" data-toggle="buttons-radio">
                            <a style="margin-left: 60px"><button class="btn" id="hour" onclick="selectionBtn(this)">시간별</button></a>
                                    <a><button class="btn" id="day" onclick="selectionBtn(this)">요일별</button></a>
                                    <a><button class="btn" id="month" onclick="selectionBtn(this)">월  별</button></a>
                            </div>
                            <form id="plantSearch" class="form-search form-horizontal pull-right" action="stats3.html">
                                <div class="input-append span12">
                                    <input name="plantSelect" type="text" class="search-query" placeholder= "">
                                    <button type="submit" class="btn" onclick="document.getElementById('plantSearch').submit();"><i class="icon-search"></i></button>
                                </div>
                            </form>
                        </div>
                    </div>

                    <!-- morris graph chart -->
                    <div class="row-fluid section">
                         <!-- block -->
                        <div class="block">
                            <div class="navbar navbar-inner block-header">
                                <div class="muted pull-left">토양 수분 분석 데이터</div>
                            </div>
                            <div class="block-content collapse in">
                                <div class="span2">
                                    <div class="btn-group" data-toggle="buttons-radio">
                                        <button class="btn" type="submit" id="tempbtn" style="height: 110px; width: 130px" onclick="buttonEvent(this);"">
                                        온도</button><br>
                                        <button class="btn" type="submit" id="moisbtn" style="height: 110px; width: 130px" onclick="buttonEvent(this);">
                                        습도</button>
                                    </div>
                                </div>
                                <div class="span10" id="soilGraph">
                                    <div id="hero-graph1" style="height: 230px;"></div>
                                </div>
                            </div>
                        </div>
                        <!-- /block -->
                    </div>
                    <div class="row-fluid section">
                         <!-- block -->
                        <div class="block">
                            <div class="navbar navbar-inner block-header">
                                <div class="muted pull-left">토양 PH 분석 데이터
                                </div>
                            </div>
                            <div class="block-content collapse in">
                                <div class="span2">
                                    <div class="btn-group" data-toggle="buttons-radio">
                                        <button class="btn" type="submit" id="soilbtn" style="height: 140px; width: 130px" onclick="buttonEvent(this)">
                                        Soil</button><br>
                                        <button class="btn" type="submit" id="phbtn" style="height: 140px; width: 130px" onclick="buttonEvent(this)">
                                        PH</button>
                                    </div>
                                </div>
                                <div class="span10">
                                    <form class="form-inline" style="margin-left: 180px;">
                                        <h5>화분선택 :&nbsp;
                                        <label class="radio-inline"><input type="radio" name="optradio" type="submit"
                                            id="section1" onclick=""> Section 1</label>
                                        <label class="radio-inline"><input type="radio" name="optradio" type="submit"
                                            id="section2" onclick=""> Section 2</label>
                                        <label class="radio-inline"><input type="radio" name="optradio" type="submit"
                                            id="section3" onclick=""> Section 3</label>
                                        </h5>
                                    </form>
                                    <div id="hero-graph2" style="height: 230px;"></div>
                                </div>
                            </div>
                        </div>
                        <!-- /block -->
                    </div>
                    <div class="row-fluid section">
                         <!-- block -->
                        <div class="block">
                            <div class="navbar navbar-inner block-header">
                                <div class="muted pull-left">조도 분석 데이터
                                </div>
                            </div>
                            <div class="block-content collapse in">
                                <div class="span12 chart">
                                    <div id="hero-bar" style="height: 250px;"></div>
                                </div>
                            </div>
                        </div>
                        <!-- /block -->
                    </div>
                </div>
            <hr>
            <footer>
                <p>System Programming Team 9. Lets Grow</p>
            </footer>
        </div>
        <!--/.fluid-container-->
        <link rel="stylesheet" href="vendors/morris/morris.css">


        <script src="vendors/jquery-1.9.1.min.js"></script>
        <script src="vendors/raphael-min.js"></script>
        <script src="vendors/morris/morris.min.js"></script>

        <script src="bootstrap/js/bootstrap.min.js"></script>

        <script src="assets/scripts.js"></script>
        <script>
            // 첫 화면 뷰 기본 선택;
        var sensorState1 = "temp";
        var sensorState2 = "soil";
        var view = "month";
        var id1 = null;
        var id2 = null;
        var check;

        function buttonEvent(obj){
            if(obj.id=="tempbtn" || obj.id=="moisbtn"){
                if(id1!=obj){
                    obj.style.background = "Green";
                    obj.style.color = "white";
                    if(id1!=null){
                        id1.style.background = '#f5f5f5';
                        id1.style.color = "black";
                    }
                    id1=obj;
                    if(id1.id=="tempbtn"){
                        sensorState1="temp";
                        alert("tempbtn");
                    }  else if(id1.id=="moisbtn"){
                        alert("moisbtn");
                        sensorState1="mois";
                    }
                    check=1;
                    soil=dataChange();
                }
            } else if(obj.id=="soilbtn" || obj.id=="phbtn"){
                if(id2!=obj){
                    obj.style.background = "Green";
                    obj.style.color = "white";
                    if(id2!=null){
                        id2.style.background = '#f5f5f5';
                        id2.style.color = "black";
                    }
                    id2=obj;
                    if(id2.id=="soilbtn"){
                        sensorState2="soil";
                        alert("soilbtn");
                    } else if(id2.id=="phbtn"){
                        alert("phbtn");
                        sensorState2="ph";
                    }
                    check=2;
                    soil=dataChange();
                }
            }
        }

        function selectionBtn(obj){
            view = obj.id;
            alert(view);
            check=1;
            soil = dataChange();
            check=2;
            soil = dataChange();
        }

        function dataChange(){
            //데이터 바꿔주는 작업해줌.
            if(check==1){ // 온도/습도 버튼을 눌렀을때 해당 되는 값으로 바꿔줌
                alert(check);
                if(sensorState1=="temp"){
                    //요일에 따른 temp값으로 변경
                    switch(view.toString()){
                        case "hour": alert(view.toString()+"요일의 temp값 넣어주기");
                        break;
                        case "day": alert(view.toString()+"요일의 temp값 넣어주기");
                        break;
                        case "month": alert(view.toString()+"요일의 temp값 넣어주기");
                        break
                        default: alert("뭐지??");
                        break;
                    }
                } else if(sensorState1=="mois"){
                    //요일에 따른 mois값으로 변경
                    switch(view.toString()){
                        case "hour": alert(view.toString()+"요일의 mois 넣어주기");
                        break;
                        case "day": alert(view.toString()+"요일의 mois 넣어주기");
                        break;
                        case "month": alert(view.toString()+"요일의 mois 넣어주기");
                        break
                        default: alert("뭐지??");
                        break;
                    }
                }
            } else if(check==2){ // 토양, ph 버튼을 눌렀을 때 해당 되는 값으로 바꿔줌
                alert(check);
                if(sensorState2=="soil"){
                    //
                    switch(view.toString()){
                        case "hour": alert(view.toString()+"요일의 soil 넣어주기");
                        break;
                        case "day": alert(view.toString()+"요일의 soil 넣어주기");
                        break;
                        case "month": alert(view.toString()+"요일의 soil 넣어주기");
                        break
                        default: alert("뭐지??");
                        break;
                    }
                } else if(sensorState2=="ph"){
                    //
                    switch(view.toString()){
                        case "hour": alert(view.toString()+"요일의 ph 넣어주기");
                        break;
                        case "day": alert(view.toString()+"요일의 ph 넣어주기");
                        break;
                        case "month": alert(view.toString()+"요일의 ph 넣어주기");
                        break
                        default: alert("뭐지??");
                        break;
                    }
                }
            }
        }

        var tax_data = [
            {"month": "2013-04", "data": 2407, "fits": 660},
            {"month": "2013-03", "data": 3351, "fits": 729},
            {"month": "2013-02", "data": 2469, "fits": 1318},
            {"month": "2013-01", "data": 2246, "fits": 461},
            {"month": "2012-12", "data": 3171, "fits": 1676},
            {"month": "2012-11", "data": 2155, "fits": 681},
            {"month": "2012-10", "data": 1226, "fits": 620},
            {"month": "2012-09", "data": 2245, "fits": 500}    
        ];

        var soil1 = [
            {"month": "2017-08", "data": 1024, "fits": 800},
            {"month": "2017-07", "data": 984, "fits": 800},
            {"month": "2017-06", "data": 876, "fits": 805},
            {"month": "2017-05", "data": 788, "fits": 877},
            {"month": "2017-04", "data": 889, "fits": 900},
            {"month": "2017-03", "data": 442, "fits": 884},
            {"month": "2017-02", "data": 560, "fits": 854},
            {"month": "2017-01", "data": 777, "fits": 800}
        ];

        var soil2 = [
            {"month": "2013-04", "data": 2407, "fits": 660},
            {"month": "2013-03", "data": 3351, "fits": 729},
            {"month": "2013-02", "data": 2469, "fits": 1318},
            {"month": "2013-01", "data": 2246, "fits": 461},
            {"month": "2012-12", "data": 3171, "fits": 1676},
            {"month": "2012-11", "data": 2155, "fits": 681},
            {"month": "2012-10", "data": 1226, "fits": 620},
            {"month": "2012-09", "data": 2245, "fits": 500}   
        ];

        Morris.Line({
            element: 'hero-graph1',
            data: soil1,
            xkey: 'month',
            xLabels: "month",
            ykeys: ['data', 'fits'],
            labels: ['Data Value', 'Goodness of fit']
        });

        Morris.Line({
            element: 'hero-graph2',
            data: soil2,
            xkey: 'month',
            xLabels: "month",
            ykeys: ['data', 'fits'],
            labels: ['Data Value', 'Goodness of fit']
        });

        Morris.Bar({
            element: 'hero-bar',
            data: [
                {day: '1', data: 136, fits: 300},
                {day: '3G', data: 1037, fits:1500},
                {day: '3GS', data: 275, fits:350},
                {day: '4', data: 380, fits:400},
                {day: '4S', data: 655, fits:666}, 
                {day: '5', data: 1571, fits:1300}
            ],
            xkey: 'day',
            ykeys: ['data','fits'],
            labels: ['Data Value', 'Goodness of fit'],
            barRatio: 0.4,
            xLabelMargin: 10,
            hideHover: 'auto'
        });

        function labelFormatter(label, series) {
            return "<div style='font-size:8pt; text-align:center; padding:2px; color:white;'>" + label + "<br/>" + Math.round(series.percent) + "%</div>";
        }
        </script>
    </body>

</html>




