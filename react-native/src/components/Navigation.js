import React, { useState } from 'react';
import { Appbar, BottomNavigation } from 'react-native-paper';
import PropTypes from 'prop-types';

import ChartsScreen from '../screens/ChartsScreen';
import SettingsScreen from '../screens/SettingsScreen';

const ChartsRoute = ({ sensorData }) => {
  return <ChartsScreen sensorData={sensorData} />;
};

const SettingsRoute = ({ writeNewSettings }) => {
  return <SettingsScreen writeNewSettings={writeNewSettings} />;
};

const initialState = {
  index: 0,
  routes: [
    { key: 'charts', title: 'Charts', icon: 'chart-areaspline' },
    { key: 'settings', title: 'Settings', icon: 'tune' },
  ],
};

const Navigation = (props) => {
  const [state, setState] = useState({ ...initialState });

  const handleIndexChange = (index) => setState({ ...state, index });
  const { sensorData, writeNewSettings } = props;

  const renderScene = BottomNavigation.SceneMap({
    charts: () => ChartsRoute({ sensorData }),
    settings: () => SettingsRoute({ writeNewSettings }),
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

Navigation.propTypes = {
  sensorData: PropTypes.array.isRequired,
  writeNewSettings: PropTypes.func.isRequired,
};

export default Navigation;
