//log(1);
function test1() {
    log(version);
    version = '2.0';
    log(version);
    log('aaa', 'b', 'c');
    'hello world';
}
function test2() {
    var ccp = new CPP(99);
    ccp.say("abc");
    //log(ccp.number)
    let ret = ccp.numberAdd();
    log(ret)
    //ccp.say();
}
var ccp1 = new CPP(10);
var ccp2 = new CPP(22);
ccp1.brother(ccp2);