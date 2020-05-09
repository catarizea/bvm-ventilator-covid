import React, { useState } from 'react';
import { Appbar, BottomNavigation } from 'react-native-paper';
import BLEManager from 'react-native-ble-manager';

import ChartsScreen from './screens/ChartsScreen';
import SettingsScreen from './screens/SettingsScreen';

const ChartsRoute = () => <ChartsScreen />;
const SettingsRoute = () => <SettingsScreen />;

const initialState = {
  index: 0,
  routes: [
    { key: 'charts', title: 'Visualize Charts', icon: 'chart-bell-curve' },
    { key: 'settings', title: 'Change Settings', icon: 'tune' },
  ],
};

const App = (props) => {
  const [state, setState] = useState({ ...initialState });

  const handleIndexChange = (index) => setState({ ...state, index });

  const renderScene = BottomNavigation.SceneMap({
    charts: ChartsRoute,
    settings: SettingsRoute,
  });

  return (
    <>
      <Appbar.Header>
        <Appbar.Content
          title={`${state.routes[state.index].title} | BVM Ventilator Covid-19`}
        />
      </Appbar.Header>
      <BottomNavigation
        navigationState={state}
        onIndexChange={handleIndexChange}
        renderScene={renderScene}
      />
    </>
  );
};

export default App;
