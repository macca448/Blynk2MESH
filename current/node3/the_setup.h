void setup() {
    pinMode(LED, OUTPUT);
    digitalWrite(LED, ON);
    pinMode(TRIG_PIN, OUTPUT);
    pinMode(ECHO_PIN, INPUT);
    pinMode(LIGHT, OUTPUT);
    digitalWrite(LIGHT, LOW);
  #ifdef PRINT
    Serial.begin(115200);
    while(!Serial){};
    Serial.println("\n\rStarting Sensors.......\n");  
  #endif
    
    sensorINTS();

    //mesh.setDebugMsgTypes( ERROR | MESH_STATUS | CONNECTION | SYNC | COMMUNICATION | GENERAL | MSG_TYPES | REMOTE ); // all types on
    mesh.setDebugMsgTypes( ERROR | STARTUP );  // set before init() so that you can see startup messages

    mesh.init( MESH_PREFIX, MESH_PASSWORD, &userScheduler, MESH_PORT );
    mesh.onReceive(&receivedCallback);
  #ifdef PRINT
    mesh.onNewConnection(&newConnectionCallback);
    mesh.onChangedConnections(&changedConnectionCallback);
    mesh.onNodeTimeAdjusted(&nodeTimeAdjustedCallback);
  #endif    
    mesh.setContainsRoot(true);
    mesh.initOTAReceive("maintank");
    //userScheduler.addTask(taskSendMessage);
    //taskSendMessage.enable();
    digitalWrite(LED, OFF);
}
