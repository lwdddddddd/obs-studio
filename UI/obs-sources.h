#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <obs.hpp>
#include <obs-frontend-api.h>

#define kMainScene "MainScene"

namespace accrecorder {
namespace source {

class Scene;
class SceneItem {
public:
	enum Type {
		kAudioInput = 0,
		kAudioOutput,
		kCamera,
		kIPCamera,
		kScreen,
		kWindow
	};

	struct Settings {
		// position
		vec2 pos;
		// scale
		vec2 scale;
		// visability
		bool hidden;
		// lock states
		bool lock;
	};

	// the id from the scene
	virtual uint64_t SceneID() const = 0;
	virtual void SetSceneID(uint64_t id) = 0;
	// the scene item name(must be unique)
	virtual std::string Name() const = 0;
	virtual void SetName(std::string &name) = 0;
	// item type string(internal)
	virtual std::string Kind() const = 0;
	// item type enum(external)
	virtual Type type() const = 0;

	// item location & scale in the scene
	virtual Settings GetSettings() const = 0;
	virtual void UpdateSettings(Settings s) = 0;
	virtual void Hide(bool hidden) = 0;
	virtual void Lock(bool lock) = 0;
	virtual void UpdateScale(vec2 sacle) = 0;
	virtual void UpdatePosition(vec2 pos) = 0;

	// get item properties
	virtual obs_data_t *Properties() const = 0;

	// should apply any changes(properties or settings)
	virtual bool ShouldApplyAnyUpdates() const = 0;

protected:
	virtual Scene *scene() const = 0;
};

// screen item
class ScreenSceneItem : public SceneItem {
public:
	ScreenSceneItem(std::string &name);
	~ScreenSceneItem();

	virtual uint64_t SceneID() const override;
	virtual void SetSceneID(uint64_t id) override;

	virtual std::string Name() const override;
	virtual void SetName(std::string &name) override;
	virtual std::string Kind() const override;
	virtual Type type() const override;

	virtual Settings GetSettings() const override;
	virtual void UpdateSettings(Settings b) override;
	virtual void Hide(bool hidden) override;
	virtual void Lock(bool lock) override;
	virtual void UpdateScale(vec2 sacle) override;
	virtual void UpdatePosition(vec2 pos) override;

	virtual obs_data_t *Properties() const override;
	virtual bool ShouldApplyAnyUpdates() const override;

	// screen index(all screens are identified by this property)
	int index;
	// capture screen method: Auto, DXGI, or WGC (default is auto)
	int capture_method;
	// show cursor(default is true)
	bool show_cursor;

protected:
	virtual Scene *scene() const override;

private:
	std::string name_;
	Type type_;
	Settings settings_;
	uint64_t scene_id_;
	bool should_apply_changes_;
};

class IPCameraSceneItem : public SceneItem {
public:
	IPCameraSceneItem(std::string &name, std::string &url, bool stopOnHide);
	~IPCameraSceneItem();

	virtual uint64_t SceneID() const override;
	virtual void SetSceneID(uint64_t id) override;
	virtual std::string Name() const override;
	virtual void SetName(std::string &name) override;
	virtual std::string Kind() const override;
	virtual Type type() const override;
	virtual Settings GetSettings() const override;
	virtual void UpdateSettings(Settings b) override;
	virtual void Hide(bool hidden) override;
	virtual void Lock(bool lock) override;
	virtual void UpdateScale(vec2 sacle) override;
	virtual void UpdatePosition(vec2 pos) override;
	virtual obs_data_t *Properties() const override;
	virtual bool ShouldApplyAnyUpdates() const override;

	void UpdateURL(std::string &url);
	void UpdateStopOnHide(bool state);

protected:
	virtual Scene *scene() const override;

private:
	std::string name_;
	Type type_;
	Settings settings_;
	uint64_t scene_id_;
	bool should_apply_changes_;
	std::string url_;
	bool stop_on_hide_;
};

class Scene {
public:
	Scene(std::string &name, obs_scene_t *src);
	Scene(const Scene &copy) = delete;
	~Scene();

	bool Attach(SceneItem *item);
	bool ApplySceneItemSettingsUpdate(SceneItem *item);

private:
	std::string name_;
	obs_scene_t *scene_;

	std::vector<SceneItem *> items_;

	obs_sceneitem_t *CreateSceneItem(obs_source_t *source,
					 obs_scene_t *scene,
					 bool sceneItemEnabled,
					 obs_transform_info *sceneItemTransform,
					 obs_sceneitem_crop *sceneItemCrop);
};

}// namespace source
}// namespace accrecorder