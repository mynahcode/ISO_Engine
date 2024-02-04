#include "iepch.h"

#include "SceneHierarchyPanel.h"
#include "IsoEngine/Scene/Components.h"

#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

// TODO: Update/clean up presentation of SceneHierarchyPanel and IsoEditor in general.

namespace IE
{

	const ImGuiTreeNodeFlags treeNodeFlags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_AllowItemOverlap;

	static void DrawVec3Control(const std::string& label, glm::vec3& values, float resetValue = 0.0f, float colWidth = 100)
	{
		ImGui::PushID(label.c_str());

		ImGui::Columns(2);
		ImGui::SetColumnWidth(0, colWidth);
		ImGui::Text(label.c_str());
		ImGui::NextColumn();

		ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0, 0 });

		float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
		ImVec2 buttonSize = { lineHeight + 3.0f, lineHeight };

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.9f, 0.2f, 0.2f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
		if (ImGui::Button("X", buttonSize))
			values.x = resetValue;
		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		ImGui::DragFloat("##X", &values.x, 0.1f, 0.0f, 0.0f, "%.2f");
		ImGui::PopItemWidth();
		ImGui::SameLine();

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.2f, 0.8f, 0.2f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.3f, 0.9f, 0.3f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.2f, 0.8f, 0.2f, 1.0f });
		if (ImGui::Button("Y", buttonSize))
			values.y = resetValue;
		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		ImGui::DragFloat("##Y", &values.y, 0.1f, 0.0f, 0.0f, "%.2f");
		ImGui::PopItemWidth();
		ImGui::SameLine();

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.2f, 0.35f, 0.9f, 1.0f });
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });
		if (ImGui::Button("Z", buttonSize))
			values.z = resetValue;
		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		ImGui::DragFloat("##Z", &values.z, 0.1f, 0.0f, 0.0f, "%.2f");
		ImGui::PopItemWidth();

		ImGui::PopStyleVar();

		ImGui::Columns(1);
		ImGui::PopID();
	}

	SceneHierarchyPanel::SceneHierarchyPanel(const Ref<Scene>& context)
	{
		SetContext(context);
	}

	void SceneHierarchyPanel::SetContext(const Ref<Scene>& context)
	{
		m_Context = context;
		m_SelectionContext = {};
	}

	void SceneHierarchyPanel::SetSelectedEntity(Entity entity)
	{

		if (m_SelectionContext)
		{
			m_PrevSelectionContext = m_SelectionContext;
			if (m_PrevSelectionContext.HasComponent<TileComponent>())
			{
				auto& tile = m_PrevSelectionContext.GetComponent<TileComponent>();
				if (tile.IsSelected)
				{
					auto& src = m_PrevSelectionContext.GetComponent<SpriteRendererComponent>().SubTextures;
					src.pop_back();
				}
				tile.IsSelected = false;
			}
		}

		m_SelectionContext = entity;
		if (m_SelectionContext.HasComponent<TileComponent>())
		{
			// If entity has a TileComponent it will always have a SpriteRendererComponent subtexture (so far).
			auto& src = m_SelectionContext.GetComponent<SpriteRendererComponent>().SubTextures;
			src.push_back(m_Context->GetSpriteTexture(14));
			auto& tile = m_SelectionContext.GetComponent<TileComponent>().IsSelected = true;
		}
	}

	void SceneHierarchyPanel::ResetSelectionContext()
	{
		if (m_SelectionContext.HasComponent<TileComponent>())
		{
			auto& tile_textures = m_SelectionContext.GetComponent<SpriteRendererComponent>().SubTextures;
			if (tile_textures.size() > 1)
			{
				ISOLOGGER_WARN("Removing Selection Texture \n");
				tile_textures.pop_back();
			}
		}
		m_SelectionContext = {};
	}

	void SceneHierarchyPanel::OnImGuiRender()
	{
		/* Scene Hierarchy Panel*/
		ImGui::Begin("Scene Hierarchy");
		m_Context->m_Registry.each([&](auto entityID)
		{
				Entity entity{ entityID, m_Context.get() };
				DrawEntityNode(entity);
		});

		if (ImGui::IsMouseDown(0) && ImGui::IsWindowHovered())
		{
			m_SelectionContext = {};
		}

		if (ImGui::BeginPopupContextWindow(0, 1 | ImGuiPopupFlags_NoOpenOverItems))
		{
			if (ImGui::MenuItem("Create Empty Entity"))
				m_Context->CreateEntity("Empty Entity");

			ImGui::EndPopup();
		}

		ImGui::End();

		/* Entity Properties Panel */
		ImGui::Begin("Entity Properties");
		if (m_SelectionContext)
		{
			DrawEntityComponents(m_SelectionContext);

			if (ImGui::Button("Add Component"))
				ImGui::OpenPopup("AddComponent");

			if (ImGui::BeginPopup("AddComponent"))
			{
				if (ImGui::MenuItem("Camera"))
				{
					if (!m_SelectionContext.HasComponent<CameraComponent>())
						m_SelectionContext.AddComponent<CameraComponent>();
					else
						ISOLOGGER_WARN("This entity already has the Camera Component!");
					ImGui::CloseCurrentPopup();
				}

				if (ImGui::MenuItem("Sprite"))
				{
					if (ImGui::MenuItem("Sprite Renderer"))
					{
						if (!m_SelectionContext.HasComponent<SpriteRendererComponent>())
							m_SelectionContext.AddComponent<SpriteRendererComponent>();
						else
							ISOLOGGER_WARN("This entity already has the Sprite Renderer Component!");
						ImGui::CloseCurrentPopup();
					}
				}

				if (ImGui::MenuItem("Tile"))
				{
					if (ImGui::MenuItem("Normal Tile"))
					{
						// TODO: Add different types of tiles/interactive tiles/trigger tiles
						if (!m_SelectionContext.HasComponent<TileComponent>())
							m_SelectionContext.AddComponent<TileComponent>();
						else
							ISOLOGGER_WARN("This entity already has the Tile Component!");
						ImGui::CloseCurrentPopup();
					}
				}

				// TODO: Update NativeScriptComponent
				if (ImGui::MenuItem("C++ Native Script"))
				{
					m_SelectionContext.AddComponent<NativeScriptComponent>();
					ImGui::CloseCurrentPopup();
				}

				ImGui::EndPopup();
			}
		}

		ImGui::End();
	}

	void SceneHierarchyPanel::DrawEntityNode(Entity entity)
	{
		auto& tag = entity.GetComponent<TagComponent>().Tag;

		ImGuiTreeNodeFlags flags = ((m_SelectionContext == entity) ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;
		bool opened = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)entity, flags, tag.c_str());
		if (ImGui::IsItemClicked())
		{
			m_SelectionContext = entity;
		}

		bool entityDeleted = false;
		if (ImGui::BeginPopupContextItem(0, 1 | ImGuiPopupFlags_MouseButtonRight))
		{
			if (ImGui::MenuItem("Delete Entity"))
				entityDeleted = true;

			ImGui::EndPopup();
		}

		if (opened)
		{
			ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow;
			bool opened = ImGui::TreeNodeEx((void*)9817239, flags, tag.c_str());
			if (opened)
				ImGui::TreePop();
			ImGui::TreePop();
		}

		if (entityDeleted)
		{
			m_Context->DestroyEntity(entity);
			if (m_SelectionContext == entity)
				m_SelectionContext = {};
		}
	}

	void SceneHierarchyPanel::DrawEntityComponents(Entity entity)
	{
		if (entity.HasComponent<TagComponent>())
		{
			auto& tag = entity.GetComponent<TagComponent>().Tag;
			char buffer[256];
			memset(buffer, 0, sizeof(buffer));
			strcpy_s(buffer, sizeof(buffer), tag.c_str()); 
			if (ImGui::InputText("Tag", buffer, sizeof(buffer)))
			{
				tag = std::string(buffer);
			}
		}

		if (entity.HasComponent<TransformComponent>())
		{
			bool open = ImGui::TreeNodeEx((void*)typeid(TransformComponent).hash_code(), treeNodeFlags, "Transform");
			if (open)
			{
				auto& tc = entity.GetComponent<TransformComponent>();
				DrawVec3Control("Translation", tc.Translation);
				glm::vec3 rotation = glm::degrees(tc.Rotation);
				DrawVec3Control("Rotation", rotation);
				tc.Rotation = glm::radians(rotation);
				DrawVec3Control("Scale", tc.Scale, 1.0f);

				ImGui::TreePop();
			}

		}

		if (entity.HasComponent<CameraComponent>())
		{
			bool open = ImGui::TreeNodeEx((void*)typeid(CameraComponent).hash_code(), treeNodeFlags, "Camera");
			ImGui::SameLine();
			if (ImGui::Button("+"))
			{
				ImGui::OpenPopup("ComponentSettings");
			}

			bool removeComponent = false;
			if (ImGui::BeginPopup("ComponentSettings"))
			{
				if (ImGui::MenuItem("Remove Component"))
					removeComponent = true;
				ImGui::EndPopup();
			}

			if (open)
			{
				auto& cameraComponent = entity.GetComponent<CameraComponent>();
				auto& camera = cameraComponent.Camera;

				ImGui::Checkbox("Primary", &cameraComponent.isPrimary);

				const char* projectionTypeStrings[] = { "Perspective", "Orthographic", "Isometric" };
				const char* currentProjectionType = projectionTypeStrings[(int)camera.GetProjectionType()];
				if (ImGui::BeginCombo("Projection", currentProjectionType))
				{
					for (int i = 0; i < 3; i++)
					{
						bool isSelected = currentProjectionType == projectionTypeStrings[i];
						if (ImGui::Selectable(projectionTypeStrings[i], isSelected))
						{
							currentProjectionType = projectionTypeStrings[i];
							camera.SetProjectionType((SceneCamera::ProjectionType)i);
						}

						if (isSelected)
							ImGui::SetItemDefaultFocus();
					}

					ImGui::EndCombo();
				}

				if (camera.GetProjectionType() == SceneCamera::ProjectionType::Perspective)
				{
					float verticalFOV = glm::degrees(camera.GetPerspectiveVerticalFOV());
					float perspectiveNear = camera.GetPerspectiveNearClip();
					float perspectiveFar = camera.GetPerspectiveFarClip();

					if (ImGui::DragFloat("Vertical FOV", &verticalFOV))
					{
						camera.SetPerspectiveVerticalFOV(verticalFOV);
					}

					if (ImGui::DragFloat("Near", &perspectiveNear))
					{
						camera.SetPerspectiveNearClip(perspectiveNear);
					}

					if (ImGui::DragFloat("Far", &perspectiveFar))
					{
						camera.SetPerspectiveFarClip(perspectiveFar);
					}
				}

				if (camera.GetProjectionType() == SceneCamera::ProjectionType::Orthographic)
				{
					float orthoSize = camera.GetOrthographicSize();
					float orthoNear = camera.GetOrthographicNearClip();
					float orthoFar = camera.GetOrthographicFarClip();

					if (ImGui::DragFloat("Size", &orthoSize))
					{
						camera.SetOrthographicSize(orthoSize);
					}

					if (ImGui::DragFloat("Near", &orthoNear))
					{
						camera.SetOrthographicNearClip(orthoNear);
					}

					if (ImGui::DragFloat("Far", &orthoFar))
					{
						camera.SetOrthographicFarClip(orthoFar);
					}

					ImGui::Checkbox("Fixed Aspect Ratio", &cameraComponent.fixedAspectRatio);
				}

				if (camera.GetProjectionType() == SceneCamera::ProjectionType::Isometric)
				{
					float orthoSize = camera.GetOrthographicSize();
					float orthoNear = camera.GetOrthographicNearClip();
					float orthoFar = camera.GetOrthographicFarClip();

					if (ImGui::DragFloat("Size", &orthoSize))
					{
						camera.SetOrthographicSize(orthoSize);
					}

					if (ImGui::DragFloat("Near", &orthoNear))
					{
						camera.SetOrthographicNearClip(orthoNear);
					}

					if (ImGui::DragFloat("Far", &orthoFar))
					{
						camera.SetOrthographicFarClip(orthoFar);
					}

					ImGui::Checkbox("Fixed Aspect Ratio", &cameraComponent.fixedAspectRatio);
				}

				ImGui::TreePop();
			}


			if (removeComponent)
				entity.RemoveComponent<SpriteRendererComponent>();
		}

		if (entity.HasComponent<SpriteRendererComponent>())
		{
			bool open = ImGui::TreeNodeEx((void*)typeid(SpriteRendererComponent).hash_code(), treeNodeFlags, "Sprite Options");
			ImGui::SameLine();
			if (ImGui::Button("+"))
			{
				ImGui::OpenPopup("ComponentSettings");
			}

			bool removeComponent = false;
			if (ImGui::BeginPopup("ComponentSettings"))
			{
				if (ImGui::MenuItem("Delete Component"))
					removeComponent = true;
				ImGui::EndPopup();
			}

			if (open)
			{
				auto& src = entity.GetComponent<SpriteRendererComponent>();
				ImGui::ColorEdit4("Color", glm::value_ptr(src.Color));


				ImVec4 border_col = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
				ImVec4 tint_col = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);

				if (src.Texture != nullptr)
				{
					ImGui::NewLine();
					ImVec2 uv_min = ImVec2(0.0f, 0.0f);                 // Top-left
					ImVec2 uv_max = ImVec2(1.0f, 1.0f);                 // Lower-right
					ImVec2 uv_tex = ImVec2(src.Texture->GetWidth(), src.Texture->GetHeight());	// (32.0, 32.0) UV coords for texture
					ImGui::Image(reinterpret_cast<void*>(src.Texture->GetRendererID()), uv_tex, uv_max, uv_min, tint_col, border_col);
					ImGui::SameLine();
					ImGui::Text("Base Texture");
				}
				if (!src.SubTextures.empty())
				{
					ImVec2 uv_texSize = ImVec2(32.0f, 32.0f);

					int i = 0;
					bool removeSubtexture = false;

					for (auto subtexture : src.SubTextures)
					{
						if (i == src.SubTextures.size() - 1 || src.SubTextures.front() == m_Context->GetSpriteTexture(13))
						{
							break;
						}
						ImGui::NewLine();
						auto subtexture_coords = subtexture->GetSubTextureCoords();
						ImVec2 uv_min = ImVec2(subtexture_coords[0].x, subtexture_coords[0].y);
						ImVec2 uv_max = ImVec2(subtexture_coords[1].x, subtexture_coords[1].y);
						ImGui::Image(reinterpret_cast<void*>(subtexture->GetTexture()->GetRendererID()), uv_texSize, uv_max, uv_min, tint_col, border_col); // min/max reversed because texture is flipped by ImGui
						ImGui::SameLine();
						ImGui::Text("Subtexture Layer %d", i + 1);

						ImGui::SameLine();
						ImGui::PushID(i);
						if (ImGui::Button("+"))
						{
							ImGui::OpenPopup("SubTextureSettings");
						}

						if (ImGui::BeginPopup("SubTextureSettings"))
						{
							if (ImGui::MenuItem("Delete"))
							{
								removeSubtexture = true; // only one subtexture can be removed at a any given  time and only one per tile
								m_SubtextureRemove = subtexture;
							}
							
							ImGui::EndPopup();
						}
						ImGui::PopID();
						i++;
					}

					if (removeSubtexture)
					{
						if (i == 0)
						{
							auto& deletedSubtexture = std::find(src.SubTextures.begin(), src.SubTextures.end(), m_SubtextureRemove);
							src.SubTextures.erase(deletedSubtexture);
							src.SubTextures.insert(src.SubTextures.begin(), m_Context->GetSpriteTexture(13));
						}
						else
						{
							auto& deletedSubtexture = std::find(src.SubTextures.begin(), src.SubTextures.end(), m_SubtextureRemove);
							src.SubTextures.erase(deletedSubtexture);
						}

						m_SubtextureRemove = {};
					}
				}


				//ImGui::SameLine();
				if (ImGui::Button("New"))
				{
					ImGui::OpenPopup("AddSprite");
				}
				if (ImGui::BeginPopup("AddSprite"))
				{
					if (ImGui::BeginMenu("Texture Sprite"))
					{
						// TODO
						ImGui::EndMenu();
					}
					if (ImGui::BeginMenu("Subtexture Sprite Layer"))
					{
						// removing border for texture ImageButtons
						tint_col = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
						ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.f, 0.f, 0.f, 0.f));
						ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.f, 0.f, 0.f, 0.f));
						ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.f, 0.f, 0.f, 0.f));
						int i = 1;
						for (auto& subtexture : m_Context->m_SpriteSheetTextures)
						{
							ImGui::PushID(i);
							ImVec2 uv_texSize = ImVec2(32.0f, 32.0f);
							auto subtexture_coords = subtexture->GetSubTextureCoords();
							ImVec2 uv_min = ImVec2(subtexture_coords[0].x, subtexture_coords[0].y);
							ImVec2 uv_max = ImVec2(subtexture_coords[1].x, subtexture_coords[1].y);
							if (ImGui::ImageButton("##", reinterpret_cast<void*>(subtexture->GetTexture()->GetRendererID()), uv_texSize, uv_max, uv_min, tint_col, border_col))
							{
								if (std::find(src.SubTextures.begin(), src.SubTextures.end(), subtexture) != src.SubTextures.end())
								{
									ISOLOGGER_WARN("Subtexture is already placed in tile!")
								}
								else if (src.SubTextures.front() != m_Context->GetSpriteTexture(13)) // use-case: tile has texture and want to add subtexture on top
								{
									src.SubTextures.emplace(src.SubTextures.end() - 1, subtexture);
								}
								else // adding a texture to tile with default texture
								{
									src.SubTextures.erase(src.SubTextures.begin());
									src.SubTextures.insert(src.SubTextures.begin(), subtexture);
								}
							}
							ImGui::PopID();

							if (i % 5 == 0)
							{
								ImGui::NewLine();
							}
							else
							{
								ImGui::SameLine();
							}

							
							i++;
						}

						ImGui::PopStyleColor(3);
						ImGui::EndMenu();
					}

					ImGui::EndPopup();
				}

				ImGui::TreePop();
			}

			if (removeComponent)
				entity.RemoveComponent<SpriteRendererComponent>();
		}

		if (entity.HasComponent<TileComponent>())
		{
			bool open = ImGui::TreeNodeEx((void*)typeid(TileComponent).hash_code(), treeNodeFlags, "Tile Options");
			ImGui::SameLine();
			if (ImGui::Button("+"))
			{
				ImGui::OpenPopup("ComponentSettings");
			}

			bool removeComponent = false;
			if (ImGui::BeginPopup("ComponentSettings"))
			{
				if (ImGui::MenuItem("Delete Component"))
					removeComponent = true;
				ImGui::EndPopup();
			}

			if (open)
			{

				// TODO: Tile dimension slider
				//auto& tile = entity.GetComponent<TileComponent>().Dimensions;
				
				ImGui::TreePop();
			}

			if (removeComponent)
			{
				entity.RemoveComponent<TileComponent>();
			}
		}
		
		if (entity.HasComponent<NativeScriptComponent>())
		{
			//auto& nsc = entity.GetComponent<NativeScriptComponent>().Instance;
		}
		ImGui::NewLine();
	}
}