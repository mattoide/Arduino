import React, {Component} from 'react';
import {Modal, Text, TouchableOpacity, View, Alert, FlatList, StyleSheet, ToastAndroid} from 'react-native';
import MaterialIcons from 'react-native-vector-icons/MaterialIcons';


export default class Modale extends Component {
    constructor(props) {
        super(props);

    this.state = {
        modalVisible: false,
        devices:[],
        connectedDevice:{}
  };
}

componentWillReceiveProps(){
  
  if(this.props.devices.length > 0){

  this.setState({devices: this.props.devices})
  this.setState({connectedDevice: this.props.connectedDevice});
  this.setModalVisible(true);

  }

}

componentDidUpdate(){
}

  setModalVisible(visible) {
    this.setState({modalVisible: visible});
    console.log(visible)
  }

  toast(item){
    ToastAndroid.showWithGravity(
        "Nome: " + item.name + "\n" + "Indirizzo: " + item.address,
        ToastAndroid.SHORT,
        ToastAndroid.CENTER,
      );
  }
  render() {

    return (
      <View style={{}}>
        <Modal
          animationType="fade" 
          transparent={true} 
          visible={this.state.modalVisible}>

          <View style={{flex:1, width:'100%', justifyContent:'center', backgroundColor:'whitesmoke', opacity:0.8}}> 
          
          <View style={{flex:0.5, borderRadius:5, borderWidth:1, marginHorizontal:'10%', borderColor:'#333333',opacity:1, backgroundColor:'black'}}>
            <View style={{backgroundColor: '#333333', flex: 1, justifyContent:'center', alignContent:'center', alignItems:'center', marginBottom:20}}>
                <Text style={{fontSize:25, color:'#988C6C', textAlign:'center'}}>Dispositivi</Text> 
            </View>

          
        <FlatList
          data={this.state.devices}
          renderItem={({item}) => 
          <View
          style={{marginVertical:5, flexDirection:'row', alignContent:'center', justifyContent:'space-around'}}
          >
  
            <TouchableOpacity
             onPress={()=>this.toast(item)}>   
            <MaterialIcons name="info" size={30} />
          </TouchableOpacity>

          <Text style={styles.devices}>{item.name != null ? item.name : "null"}</Text>


          <TouchableOpacity
          onPress={() => {
            this.setModalVisible(false)
            this.props.connectTo(item)

          }}>
            {this.state.connectedDevice.id == item.id ? <MaterialIcons name="bluetooth-connected" color={'green'} size={30} /> : <MaterialIcons name="bluetooth-connected" size={30} />}
          </TouchableOpacity>
          </View>

        }
        />
      <TouchableOpacity 
                onPress={() => {
                  this.setModalVisible(false);
                  this.props.connectTo();
                }}>
                <Text style={{fontSize:20, color:'#988C6C', textAlign:'center'}}>CHIUDI</Text>
              </TouchableOpacity>
            </View>

        </View>

        </Modal>

        {/* <TouchableOpacity
          onPress={() => {
            this.setModalVisible(true);
          }}>
          <Text>Show Modal</Text>
        </TouchableOpacity> */}
      </View>
    );
  }
}

const styles = StyleSheet.create({
    devices:{
        color:"red"
    }
});